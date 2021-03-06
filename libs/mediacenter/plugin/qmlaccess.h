/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef QMLACCESS_H
#define QMLACCESS_H

#include "mediacenter/playlistmodel.h"
#include "mediacenter/backendsmodel.h"
#include "mediacenter/abstractbrowsingbackend.h"
#include "mediacenter/singletonfactory.h"
#include "mediacenter/datasourcesloader.h"
#include "mediacenter/mpris2/mediaplayer2player.h"

class QMLAccess : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PlaylistModel *playlistModel READ playlistModel NOTIFY playlistModelChanged)
    Q_PROPERTY(BackendsModel *backendsModel READ backendsModel NOTIFY backendsModelChanged)
    Q_PROPERTY(QObject *currentBrowsingBackend MEMBER m_currentBrowsingBackend NOTIFY currentBrowsingBackendChanged)
    Q_PROPERTY(MediaPlayer2Player *mpris2PlayerAdaptor READ mpris2PlayerAdaptor NOTIFY mpris2PlayerAdaptorChanged)

Q_SIGNALS:
    void playlistModelChanged();
    void backendsModelChanged();
    void currentBrowsingBackendChanged();
    void mpris2PlayerAdaptorChanged();

public:
    explicit QMLAccess(QObject *parent = 0);
    PlaylistModel* playlistModel() {
        return m_playlistModel;
    }
    BackendsModel* backendsModel() {
        return m_backendsModel;
    }
    MediaPlayer2Player* mpris2PlayerAdaptor() {
        return m_mpris2PlayerAdaptor;
    }

private:

    PlaylistModel* m_playlistModel;
    BackendsModel* m_backendsModel;
    QObject* m_currentBrowsingBackend;
    MediaPlayer2Player* m_mpris2PlayerAdaptor;
};

#endif // QMLACCESS_H
