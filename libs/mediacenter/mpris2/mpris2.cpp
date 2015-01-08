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

#include "mpris2.h"
#include "mediaplayer2.h"
#include "mediaplayer2player.h"
#include "mediaplayer2tracklist.h"
#include "mediacenter/medialibrary.h"
#include "mediacenter/pmcmedia.h"

#include <QDBusConnection>
#include <unistd.h>

Mpris2::Mpris2(QSharedPointer<PlaylistModel> playlistModel, QObject* parent)
    : QObject(parent)
{
    QString mspris2Name("org.mpris.MediaPlayer2." + QLatin1String("plasma-mediacenter"));

    bool success = QDBusConnection::sessionBus().registerService(mspris2Name);

    // If the above failed, it's likely because we're not the first instance
    // or the name is already taken. In that event the MPRIS2 spec wants the
    // following:
    if (!success) {
        success = QDBusConnection::sessionBus().registerService(mspris2Name + ".instance" + QString::number(getpid()));
    }

    if (success) {
        m_mp2 = new MediaPlayer2(this);
        m_mp2p = new MediaPlayer2Player(this);
        m_mp2tl = new MediaPlayer2Tracklist(playlistModel, this);

        QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this, QDBusConnection::ExportAdaptors);

        connect(m_mp2, SIGNAL(raisePMC()), this, SIGNAL(raisePMC()));
    }
}

Mpris2::~Mpris2()
{
}

MediaPlayer2Player* Mpris2::getMediaPlayer2Player()
{
    return m_mp2p;
}

QString Mpris2::getCurrentTrackId()
{
    if (m_mp2tl->currentIndex() != -1) {
        return m_mp2tl->currentTrackId().path();
    }

    QSharedPointer<PmcMedia> media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(m_mp2p->currentTrack());
    if (media) {
        return QString("/org/kde/plasmamediacenter/tid_") + media->sha();
    }

    return QString("/org/mpris/MediaPlayer2/TrackList/NoTrack");
}

QVariantMap Mpris2::getMetadataOf(const QString &url)
{
    QVariantMap metadata = getMetadataOf(url, getCurrentTrackId());
    return metadata;
}

QVariantMap Mpris2::getMetadataOf(const QString &url, const QString& trackId)
{
    QVariantMap metadata;
    QSharedPointer<PmcMedia> media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(url);
    if (media) {
        metadata["mpris:trackid"] = QVariant::fromValue<QDBusObjectPath>(QDBusObjectPath(trackId));
        metadata["mpris:length"] = qlonglong(media->duration())*1000000;
        //convert seconds into micro-seconds
        metadata["xesam:title"] = media->title();
        metadata["xesam:url"] = media->url();
        metadata["xesam:album"] = media->album();
        metadata["xesam:artist"] = QStringList(media->artist());
        metadata["xesam:genre"] = QStringList(media->genre());
    }

    return metadata;
}
