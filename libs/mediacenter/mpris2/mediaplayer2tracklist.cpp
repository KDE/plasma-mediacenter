/***************************************************************************
 *   Copyright 2014 Ashish Madeti <ashishmadeti@gmail.com>                 *
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

#include "mediaplayer2tracklist.h"

#include <QDebug>

static const QString playlistTidPrefix("/org/kde/plasmamediacenter/playlist/");

MediaPlayer2Tracklist::MediaPlayer2Tracklist(QSharedPointer<PlaylistModel> playlistModel, QObject *parent)
    : QDBusAbstractAdaptor(parent),
      m_playlistModel(playlistModel),
      tidCounter(0)
{
    connect(m_playlistModel.data(), SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(rowsInsertedInModel(QModelIndex,int,int)));
    connect(m_playlistModel.data(), SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(rowsRemovedFromModel(QModelIndex,int,int)));
    connect(m_playlistModel.data(), SIGNAL(modelReset()), this, SLOT(resetTrackIds()));
    connect(m_playlistModel.data(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)), this, SLOT(rowsMovedInModel(QModelIndex,int,int,QModelIndex,int)));

    for (int i = 0; i < m_playlistModel->rowCount(); i++) {
        m_orderedTrackIds << QDBusObjectPath(playlistTidPrefix + QString::number(tidCounter++));
    }
}

MediaPlayer2Tracklist::~MediaPlayer2Tracklist()
{
}

bool MediaPlayer2Tracklist::CanEditTracks() const
{
    return false;
}

QList<QDBusObjectPath> MediaPlayer2Tracklist::Tracks() const
{
    return m_orderedTrackIds;
}

void MediaPlayer2Tracklist::rowsInsertedInModel(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);

    for (int i = start; i <= end; i++) {
        m_orderedTrackIds.insert(i, QDBusObjectPath(playlistTidPrefix + QString::number(tidCounter++)));
    }
}

void MediaPlayer2Tracklist::rowsRemovedFromModel(const QModelIndex &parent, int start, int end)
{
    Q_UNUSED(parent);

    for (int i = start; i <= end; i++) {
        m_orderedTrackIds.takeAt(i);
    }
}

void MediaPlayer2Tracklist::rowsMovedInModel(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                             const QModelIndex &destinationParent, int movedBeforeRow)
{
    Q_UNUSED(sourceParent);
    Q_UNUSED(destinationParent);

    int destinationRow = (movedBeforeRow > sourceStart) ? movedBeforeRow - 1 : movedBeforeRow;
    for (int i = 0, index = sourceStart; index <= sourceEnd; i++, index++) {
        m_orderedTrackIds.move(index, destinationRow + i);
    }
}

void MediaPlayer2Tracklist::resetTrackIds()
{
    m_orderedTrackIds.clear();
    for (int i = 0; i < m_playlistModel->rowCount(); i++) {
        m_orderedTrackIds << QDBusObjectPath(playlistTidPrefix + QString::number(tidCounter++));
    }
}
