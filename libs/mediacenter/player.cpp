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

#include <QHash>

using namespace MediaCenter;

class Player::PlayerPrivate
{
public:
    PlayerPrivate(Player *q) : q(q),
    sshowTime(3)
    {}

    Player *q;
    qint64 sshowTime;
    QHash<Mode, bool> activeModes;
    QHash<Mode, Media> currentMedias;
    QHash<Mode, PlaybackState> playbackStates;
};

Player::Player(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args), d(new PlayerPrivate(this))
{}

Player::~Player()
{
    delete d;
}

Media Player::currentMedia(Mode mode) const
{
    return d->currentMedias.value(mode);
}

void Player::setCurrentMedia(const MediaCenter::Media& media, Mode mode)
{
    d->currentMedias.insert(mode, media);
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

PlaybackState Player::playbackState(Mode mode) const
{
    if (!d->playbackStates.contains(mode)) {
        return MediaCenter::StoppedState;
    }

    return d->playbackStates.value(mode);
}

void Player::setPlaybackState(PlaybackState state, Mode mode)
{
    if (playbackState(mode) == state) {
        return;
    }
    d->playbackStates.insert(mode, state);
    emit playbackStateChanged(state, mode);
}

bool Player::isModeActive(Mode mode) const
{
    if (!d->activeModes.contains(mode)) {
        return false;
    }

    return d->activeModes.value(mode);
}

void Player::setModeActive(Mode mode, bool set)
{
    if (d->activeModes.contains(mode) && d->activeModes.value(mode) == set) {
        return;
    }

    d->activeModes.insert(mode, set);
    emit activeModeChanged(mode, set);
}

void Player::setPlayerType(const MediaType &type)
{
    Q_UNUSED(type);
}
