/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "playlistwidget.h"
#include "utils/utils.h"
#include "playlistdelegate.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QTreeView>
#include <QIcon>
#include <QMimeData>
#include <QPalette>
#include <QFileInfo>

// QtDBus
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

// KDE
#include <KDebug>
#include <KConfigGroup>
#include <KComboBox>
#include <KMimeType>

// Plasma
#include <Plasma/TreeView>
#include <Plasma/DataEngine>
#include <Plasma/DataEngineManager>
#include <Plasma/Service>
#include <Plasma/ServiceJob>
#include <Plasma/Delegate>
#include <Plasma/ComboBox>
#include <Plasma/Theme>

// Taglib
#include <taglib/fileref.h>
#include <taglib/tag.h>

///////////////// PlaylistWidget ///////////////////////

PlaylistWidget::PlaylistWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      m_treeView(new Plasma::TreeView(this)),
      m_playlistEngine(0),
      m_coverEngine(0),
      m_model(new QStandardItemModel(this)),
      m_pupdater(new PlaylistUpdater(this)),
      m_cupdater(new CoverUpdater(this)),
      m_multiplePlaylists(false)
{
    // here we try to load the playlist engine
    m_playlistEngine = MediaCenter::loadEngineOnce("playlist");
    kDebug() << m_playlistEngine;
    connect (m_playlistEngine, SIGNAL(sourceAdded(const QString &)), this, SLOT(slotPlaylistAdded(const QString &)));

    m_coverEngine = MediaCenter::loadEngineOnce("coverfetcher");
    connect (m_coverEngine, SIGNAL(sourceAdded(const QString &)), this, SLOT(slotCoverReady(const QString &)));

    m_treeView->setModel(m_model);

    PlaylistDelegate *delegate = new PlaylistDelegate(this);
    connect(delegate, SIGNAL(removeRequested(const QModelIndex &)), this, SLOT(removeFromPlaylist(const QModelIndex &)));
    connect(delegate, SIGNAL(reloadRequested(const QModelIndex &)), this, SLOT(reloadCover(const QModelIndex &)));

    m_treeView->nativeWidget()->setItemDelegate(delegate);
    m_treeView->nativeWidget()->setHeaderHidden(true);
    m_treeView->nativeWidget()->setRootIsDecorated(false);
    QPalette p = m_treeView->nativeWidget()->palette();
    p.setColor(QPalette::Base, Qt::transparent);
    p.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    m_treeView->nativeWidget()->setPalette(p);

    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));

    m_comboBox = new Plasma::ComboBox(this);
    connect (m_comboBox->nativeWidget(), SIGNAL(currentIndexChanged(const QString &)), this, SLOT(showPlaylist(const QString &)));
    foreach (const QString &source, m_playlistEngine->sources()) {
        if (source == "currentPlaylist") {
            continue;
        }
        m_comboBox->addItem(source);
    }

    if (!m_playlistEngine->sources().count()) {
        m_playlistEngine->query("New Playlist");
    }

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(m_comboBox);
    layout->addItem(m_treeView);
    setLayout(layout);

    setAcceptDrops(true);
    m_treeView->setAcceptDrops(false);
}

PlaylistWidget::~PlaylistWidget()
{
    Plasma::DataEngineManager::self()->unloadEngine("playlist");
    Plasma::DataEngineManager::self()->unloadEngine("coverfetcher");
}

void PlaylistWidget::showPlaylist(const QString &playlistName)
{
    m_playlistEngine->connectSource(playlistName, m_pupdater);

    m_model->clear();
    QStringList files;

    foreach (const QVariant &tracks, m_playlistEngine->query(playlistName)) {
        files << tracks.toStringList();
        kDebug() << tracks;
    }

    foreach (const QString &track, files) {
        TagLib::FileRef ref(track.toLatin1());
        QStandardItem *item = new QStandardItem(ref.tag()->title().toCString(true));
        item->setEditable(false);
        if (item->text().isEmpty()) {
            item->setText(QFileInfo(track).baseName());
        }
        item->setData(ref.tag()->artist().toCString(true), PlaylistDelegate::ArtistRole);
        item->setData(track, PlaylistDelegate::FilePathRole);
        item->setData(ref.tag()->album().toCString(true), PlaylistDelegate::AlbumRole);

        // cover retrival
        QString coverSource = ref.tag()->artist().toCString(true);
        coverSource.append("|");
        coverSource.append(ref.tag()->album().toCString(true));
        item->setData(coverSource, CoverSourceRole);
        Plasma::DataEngine::Data covers = m_coverEngine->query(coverSource);
        m_coverEngine->connectSource(coverSource, m_cupdater);
        if (!covers.isEmpty()) {
            item->setData(covers["medium"].value<QPixmap>(), Qt::DecorationRole);
        }

        m_model->appendRow(item);
    }

    Plasma::Service *playlistService = m_playlistEngine->serviceForSource(playlistName);
    if (!playlistService) {
        kDebug() << "invalid service";
        return;
    }

    KConfigGroup op = playlistService->operationDescription("setCurrent");
    playlistService->startOperationCall(op); 
}

void PlaylistWidget::slotPlaylistAdded(const QString &source)
{
    if (source == "currentPlaylist") {
        return;
    }

    m_comboBox->addItem(source);
//    if (m_comboBox->nativeWidget()->count() == 1) {
//        showPlaylist(source);
//    }
}

void PlaylistWidget::slotCoverReady(const QString &source)
{
    kDebug() << "source cover ready" << source;
    for (int i = 0; i < m_model->rowCount(); i++) {
        QStandardItem *item = m_model->item(i);
        if (item->data(CoverSourceRole).toString() == source) {
            Plasma::DataEngine::Data covers = m_coverEngine->query(source);
            if (!covers.isEmpty()) {
                kDebug() << "setting cover";
                item->setData(covers["medium"].value<QPixmap>(), Qt::DecorationRole);
                break;
            }
        }
    }
}

void PlaylistWidget::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    Plasma::Service *playlistService = m_playlistEngine->serviceForSource(m_comboBox->nativeWidget()->currentText());
    if (!playlistService) {
        kDebug() << "invalid service";
        return;
    }

    kDebug() << "adding to playlist";
    KConfigGroup op = playlistService->operationDescription("add");
    foreach (const KUrl &url, event->mimeData()->urls()) {
        KMimeType::Ptr mime = KMimeType::findByUrl(url);
        if (mime->name().indexOf("video/") != -1 || mime->name().indexOf("audio/") != -1 || mime->name().indexOf("image/") != -1) {
            op.writeEntry("path", url.path());
            playlistService->startOperationCall(op);
        }
    }

}

void PlaylistWidget::playlistUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(data)

    kDebug() << "data updated";

    kDebug() << source << "updated for PlaylistEngine";
    showPlaylist(source);
}

void PlaylistWidget::coverUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(data)

    kDebug() << "data updated";

    kDebug() << "updated cover for" << source;
    slotCoverReady(source);
}

void PlaylistWidget::updateColors()
{
    QPalette p = m_treeView->nativeWidget()->palette();
    p.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    m_treeView->nativeWidget()->setPalette(p);
}

void PlaylistWidget::removeFromPlaylist(const QModelIndex &index)
{
    Plasma::Service *playlistService = m_playlistEngine->serviceForSource(m_comboBox->nativeWidget()->currentText());
    if (!playlistService) {
        return;
    }

    KConfigGroup op = playlistService->operationDescription("remove");
    op.writeEntry("path", index.data(PlaylistDelegate::FilePathRole).toString());
    playlistService->startOperationCall(op);
}

void PlaylistWidget::setMultiplePlaylistsEnabled(bool enabled)
{
    m_multiplePlaylists = true;
    m_comboBox->setVisible(enabled);
}

bool PlaylistWidget::multiplePlaylistsEnabled()
{
    return m_multiplePlaylists;
}

void PlaylistWidget::reloadCover(const QModelIndex &index)
{
    const QString sourceName = index.data(PlaylistDelegate::ArtistRole).toString() +
                               "|" + index.data(PlaylistDelegate::AlbumRole).toString();

    Plasma::Service *coverService = m_coverEngine->serviceForSource(sourceName);
    if (!coverService) {
        return;
    }

    KConfigGroup op = coverService->operationDescription("reload");
    coverService->startOperationCall(op);
}

QStringList PlaylistWidget::medias()
{
    QString playlistName = m_comboBox->nativeWidget()->currentText();
    QStringList medias;

    foreach (const QVariant &tracks, m_playlistEngine->query(playlistName)) {
        medias << tracks.toStringList();
    }

    return medias;
}

int PlaylistWidget::length()
{
    return m_model->rowCount();
}
