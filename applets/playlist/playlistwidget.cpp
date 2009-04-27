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

// Qt
#include <QGraphicsLinearLayout>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>

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

PlaylistWidget::PlaylistWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      m_treeView(new Plasma::TreeView(this)),
      m_playlistEngine(0),
      m_model(new QStandardItemModel(this))
{
    // here we try to load the playlist engine
    m_playlistEngine = Plasma::DataEngineManager::self()->engine("playlist");
    if (!m_playlistEngine->isValid()) {
        m_playlistEngine = Plasma::DataEngineManager::self()->loadEngine("playlist");
        if (!m_playlistEngine->isValid()) {
            kWarning() << "unable to load playlist engine";
        }
    }

    m_treeView->setModel(m_model);
    m_interface = new QDBusInterface("org.kde.PlaylistEngine", "/Playlist", QString(), QDBusConnection::sessionBus(), this);


    Plasma::ComboBox *comboBox = new Plasma::ComboBox(this);
    foreach (const QString &source, m_playlistEngine->sources()) {
        comboBox->addItem(source);
    }
    connect (comboBox->nativeWidget(), SIGNAL(currentIndexChanged(const QString &)), this, SLOT(showPlaylist(const QString &)));
    comboBox->nativeWidget()->setCurrentIndex(0);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(comboBox);
    layout->addItem(m_treeView);
    setLayout(layout);
}

PlaylistWidget::~PlaylistWidget()
{
}

void PlaylistWidget::showPlaylist(const QString &playlistName)
{
    m_model->clear();
    QStringList files;

    foreach (const QVariant &tracks, m_playlistEngine->query(playlistName)) {
        files << tracks.toStringList();
        kDebug() << tracks;
    }

    foreach (const QString &track, files) {
        m_model->appendRow(new QStandardItem(track));
    }

    Plasma::Service *playlistService = m_playlistEngine->serviceForSource(playlistName);
    if (!playlistService) {
        kDebug() << "invalid service";
    }

    KConfigGroup op = playlistService->operationDescription("setCurrent");
    Plasma::ServiceJob *job = playlistService->startOperationCall(op);
    connect (job, SIGNAL(finished(KJob*)), this, SLOT(jobCompleted())) ;   
}

void PlaylistWidget::jobCompleted()
{
    kDebug() << "service completed operation";
}
