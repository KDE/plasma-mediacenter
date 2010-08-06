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
#include "treeview.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsProxyWidget>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QFileInfo>

// QtDBus
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>

// KDE
#include <KDebug>
#include <KConfigGroup>
#include <KComboBox>
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
      m_treeView(new TreeView),
      m_playlistEngine(0),
      m_coverEngine(0),
      m_model(new QStandardItemModel(this)),
      m_indicator(0),
      m_pupdater(new PlaylistUpdater(this)),
      m_cupdater(new CoverUpdater(this)),
      m_multiplePlaylists(false),
      m_mediaTypeToShow(MediaCenter::Video)
{

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(m_treeView);
    proxy->setAcceptDrops(false);
    connect (m_treeView, SIGNAL(activated(QModelIndex)), this, SLOT(slotMediaActivated(QModelIndex)));

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

    m_treeView->setItemDelegate(delegate);

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
    layout->addItem(proxy);
    setLayout(layout);

    setAcceptDrops(true);

    layout->setContentsMargins(0, 0, 0, 0);
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
    QStringList temp;
    QStringList files;

    foreach (const QVariant &medias, m_playlistEngine->query(playlistName)) {
        temp << medias.toStringList();
    }

    //This part forces the playlist to only show a certain type of media
    foreach (QString file, temp) {
        if (MediaCenter::getType(file) == m_mediaTypeToShow) {
            files << file;
        }
    }

    foreach (const QString &media, files) {
        TagLib::FileRef ref(media.toLatin1());
        QString name = ref.isNull() ? QFileInfo(media).fileName() : ref.tag()->title().toCString(true);
        QStandardItem *item = new QStandardItem(name);
        item->setEditable(false);
        item->setData(media, PathRole);

        if (!ref.isNull()) {
            item->setData(ref.tag()->artist().toCString(true), PlaylistDelegate::ArtistRole);
            item->setData(media, PlaylistDelegate::FilePathRole);
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
    kWarning() << "show Playlist " << playlistName;
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
    append(event->mimeData()->urls());
}

void PlaylistWidget::append(const QList<QUrl> &list)
{
    Plasma::Service *playlistService = m_playlistEngine->serviceForSource(m_comboBox->nativeWidget()->currentText());
    if (!playlistService) {
        kDebug() << "invalid service";
        return;
    }

    QList<MediaCenter::Media> medias;
    kDebug() << "adding to playlist";
    KConfigGroup op = playlistService->operationDescription("add");
    foreach (const KUrl &url, list) {
        MediaCenter::MediaType type = MediaCenter::getType(url.path());
        if (type != MediaCenter::Invalid) {
            op.writeEntry("path", url.path());
            playlistService->startOperationCall(op);

            MediaCenter::Media media;
            media.first = type;
            media.second = url.path();
            medias << media;
        }
    }
    emit mediasAppended(medias);
}

void PlaylistWidget::playlistUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(data)
    showPlaylist(source);
}

void PlaylistWidget::coverUpdated(const QString &source, const Plasma::DataEngine::Data &data)
{
    Q_UNUSED(data)
    slotCoverReady(source);
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

QList<MediaCenter::Media> PlaylistWidget::medias(const MediaCenter::MediaType &type) const
{
    QString playlistName = m_comboBox->nativeWidget()->currentText();
    QList<MediaCenter::Media> medias;
    foreach (const QVariant &tracks, m_playlistEngine->query(playlistName)) {
        foreach (const QString &mediaString, tracks.toStringList()) {
            MediaCenter::Media media;
            MediaCenter::MediaType typeOfCurrent = MediaCenter::getType(mediaString);
            if (typeOfCurrent == MediaCenter::Invalid) {
                continue;
            }
            if (typeOfCurrent == type) {
                media.first = typeOfCurrent;
                media.second = mediaString;
                medias << media;
            }
        }
    }
    return medias;
}

int PlaylistWidget::length()
{
    return m_model->rowCount();
}

void PlaylistWidget::slotMediaActivated(const QModelIndex &index)
{
    kDebug() << "activated" << index.data(PathRole).toString();
    MediaCenter::MediaType type = MediaCenter::getType(index.data(PathRole).toString());
    if (type != MediaCenter::Invalid) {
        MediaCenter::Media media;
        media.first = type;
        media.second = index.data(PathRole).toString();
        emit mediaActivated(media);
    }
}

void PlaylistWidget::setCurrentPlaylist(QString playlistName)
{
    m_comboBox->nativeWidget()->addItem(playlistName);
}

void PlaylistWidget::setMediaTypeToShow(const MediaCenter::MediaType &type)
{
    m_mediaTypeToShow = type;
    foreach (const QString &source, m_playlistEngine->sources()) {
        showPlaylist(source);
    }
}
