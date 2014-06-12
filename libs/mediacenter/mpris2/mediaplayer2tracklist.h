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

#ifndef MEDIAPLAYER2TRACKLIST_H
#define MEDIAPLAYER2TRACKLIST_H

#include "mediacenter/mediacenter_export.h"
#include "playlistmodel.h"

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QSharedPointer>
#include <QStringList>

class MEDIACENTER_EXPORT MediaPlayer2Tracklist : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.TrackList") // Docs: http://specifications.freedesktop.org/mpris-spec/latest/Track_List_Interface.html

    Q_PROPERTY(QList<QDBusObjectPath> Tracks READ Tracks)
    Q_PROPERTY(bool CanEditTracks READ CanEditTracks)

public:
    explicit MediaPlayer2Tracklist(QSharedPointer<PlaylistModel> playlistModel, QObject* parent = 0);
    ~MediaPlayer2Tracklist();

    bool CanEditTracks() const;
    QList<QDBusObjectPath> Tracks() const;

private slots:
    void rowsInsertedInModel(const QModelIndex &parent, int start, int end);
    void rowsRemovedFromModel(const QModelIndex &parent, int start, int end);
    void rowsMovedInModel(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                          const QModelIndex &destinationParent, int movedBeforeRow);
    void resetTrackIds();

private:
    QSharedPointer<PlaylistModel> m_playlistModel;
    QList<QDBusObjectPath> m_orderedTrackIds;

    int tidCounter;
};

#endif // MEDIAPLAYER2TRACKLIST_H
