/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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

#include <Phonon/MediaObject>
#include <Phonon/AudioOutput>
#include <Phonon/Path>

#include <KDebug>

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
    QHash<Mode, QList<Media> > medias;
    QHash<Mode, Phonon::MediaObject*> mediaObjects;
    QHash<Mode, Phonon::AudioOutput*> audioOutputs;
    MediaCenter::Mode currentMode;
};

Player::Player(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args), d(new PlayerPrivate(this))
{}

Player::~Player()
{
    delete d;
}

void Player::setCurrentMode(MediaCenter::Mode mode)
{
    d->currentMode = mode;
}

MediaCenter::Mode Player::currentMode() const
{
    return d->currentMode;
}

Phonon::MediaObject* Player::mediaObject(MediaCenter::Mode mode)
{
    if (mode == MediaCenter::PictureMode) {
        return 0;
    }

    if (!d->mediaObjects.contains(mode)) {
        Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);
        Phonon::createPath(mediaObject, audioOutput(mode));
        d->mediaObjects.insert(mode, mediaObject);
    }

    return d->mediaObjects.value(mode);
}

Phonon::AudioOutput* Player::audioOutput(MediaCenter::Mode mode)
{
    if (mode == MediaCenter::PictureMode) {
        return 0;
    }

    if (!d->audioOutputs.contains(mode)) {
        Phonon::Category category = Phonon::NoCategory;
        if (mode == MediaCenter::MusicMode) {
            category = Phonon::MusicCategory;
        } else if (mode == MediaCenter::VideoMode) {
            category = Phonon::VideoCategory;
        }

        Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(category, this);
        d->audioOutputs.insert(mode, audioOutput);
    }

    return d->audioOutputs.value(mode);
}

void Player::seek(MediaCenter::Mode mode, qint64 time)
{
    Phonon::MediaObject *mediaObject = this->mediaObject(mode);
    if (!mediaObject) {
        return;
    }

    mediaObject->seek(time);
}

Media Player::currentMedia(Mode mode) const
{
    return d->currentMedias.value(mode);
}

void Player::setCurrentMedia(const MediaCenter::Media& media, Mode mode)
{
    d->currentMedias.insert(mode, media);
}

void Player::setMediaQueue(MediaCenter::Mode mode, const QList<Media> &medias)
{
    d->medias.insert(mode, medias);
}

void Player::clearQueue(MediaCenter::Mode mode)
{
    stop(mode);
    setMediaQueue(mode, QList<Media>());
}

void Player::enqueueMedia(MediaCenter::Mode mode, const QList<Media> &medias)
{
    QList<Media> storedMedias = d->medias.value(mode);
    storedMedias.append(medias);
    d->medias.insert(mode, storedMedias);
}

QList<Media> Player::mediaQueue(MediaCenter::Mode mode) const
{
    return d->medias.value(mode);
}

void Player::setSlideshowInterval(const qint64 time)
{
    d->sshowTime = time;
    emit slideShowTimeChanged(d->sshowTime);
}

qint64 Player::slideShowInterval() const
{
    return d->sshowTime;
}

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
    PlaybackState oldState = d->playbackStates.value(mode);
    d->playbackStates.insert(mode, state);
    emit playbackStateChanged(oldState, state, mode);
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

void Player::setVolume(MediaCenter::Mode mode, int volume)
{
    Phonon::AudioOutput *audioOutput = this->audioOutput(mode);
    if (!audioOutput) {
        return;
    }

    audioOutput->setVolume(volume / 100);
}

qreal Player::volume(MediaCenter::Mode mode)
{
    Phonon::AudioOutput *audioOutput = this->audioOutput(mode);
    if (!audioOutput) {
        return 0;
    }

    return audioOutput->volume();
}
