/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
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

#ifndef MEDIACENTER_MPRIS2_H
#define MEDIACENTER_MPRIS2_H

#include "mediacenter_export.h"
#include "mediacenter/playlistmodel.h"

#include <QObject>
#include <QSharedPointer>
#include <QVariantMap>

class MediaPlayer2;
class MediaPlayer2Player;
class MediaPlayer2Tracklist;

class MEDIACENTER_EXPORT Mpris2 : public QObject
{
    Q_OBJECT

public:
    explicit Mpris2(PlaylistModel* playlistModel, QObject* parent = 0);
    ~Mpris2();

    MediaPlayer2Player* getMediaPlayer2Player();
    QString getCurrentTrackId();
    QVariantMap getMetadataOf(const QString& url);
    QVariantMap getMetadataOf(const QString& url, const QString& trackId);

signals:
    void raisePMC() const;

private:
    MediaPlayer2 *m_mp2;
    MediaPlayer2Player *m_mp2p;
    MediaPlayer2Tracklist *m_mp2tl;
};

#endif //MEDIACENTER_MPRIS2_H
