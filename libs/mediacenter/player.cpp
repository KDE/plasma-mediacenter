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
#include "player.h"

using namespace MediaCenter;

class Player::PlayerPrivate
{
public:
    PlayerPrivate(Player *q) : q(q),
    sshowTime(3),
    activeVideo(false),
    activeMusic(false),
    activePicture(false)
    {}

    Player *q;
    qint64 sshowTime;
    bool activeVideo;
    bool activeMusic;
    bool activePicture;
};

Player::Player(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args), d(new PlayerPrivate(this))
{}

Player::~Player()
{
    delete d;
}

Media Player::currentVideoMedia() const
{
    return Media();
}

Media Player::currentMusicMedia() const
{
    return Media();
}

Media Player::currentPictureMedia() const
{
    return Media();
}

void Player::setVideoQueue(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::enqueueVideos(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::setMusicQueue(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::enqueueMusic(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::setPictureQueue(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::enqueuePictures(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}
//
//void Player::enqueue(int index, const QList<Media> &sources)
//{
//    Q_UNUSED(index);
//    Q_UNUSED(sources);
//}

void Player::setSlideshowInterval(const qint64 time)
{
    d->sshowTime = time;
    emit slideShowTimeChanged(d->sshowTime);
}

qint64 Player::slideShowInterval() const
{
    return d->sshowTime;
}

void Player::playVideoMedia(const MediaCenter::Media &media)
{
    Q_UNUSED(media);
}

void Player::playAllVideoMedia()
{}

void Player::playPictureMedia(const MediaCenter::Media &media)
{
    Q_UNUSED(media);
}

void Player::playAllPictureMedia()
{}

void Player::playMusicMedia(const MediaCenter::Media &media)
{
    Q_UNUSED(media);
}

void Player::playAllMusicMedia()
{}

bool Player::isVideoActive()
{
    return d->activeVideo;
}

void Player::setVideoActive(const bool set)
{
    if (d->activeVideo == set) {
        return;
    }

    d->activeVideo = set;
    emit activeVideoStateChanged(d->activeVideo);
}

bool Player::isMusicActive()
{
    return d->activeMusic;
}

void Player::setMusicActive(const bool set)
{
    if (d->activeMusic == set) {
        return;
    }

    d->activeMusic = set;
    emit activeMusicStateChanged(d->activeMusic);
}

bool Player::isPictureActive()
{
    return d->activePicture;
}

void Player::setPictureActive(const bool set)
{
    if (d->activePicture == set) {
        return;
    }

    d->activePicture = set;
    emit activePictureStateChanged(d->activePicture);
}

void Player::setPlayerType(const MediaType &type)
{
    Q_UNUSED(type);
}
