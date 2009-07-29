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
#include "playbackcontrol.h"

using namespace MediaCenter;

class PlaybackControl::PlaybackControlPrivate {
public:
    PlaybackControlPrivate(PlaybackControl *q) : q(q), mediaObject(0)
    {}

    PlaybackControl *q;
    Phonon::MediaObject *mediaObject;
};

PlaybackControl::PlaybackControl(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args),
d(new PlaybackControlPrivate(this))
{
}

PlaybackControl::~PlaybackControl()
{}

void PlaybackControl::playingStateChanged(MediaCenter::State state)
{
    Q_UNUSED(state)
}

void PlaybackControl::currentSeekChanged(qreal seek)
{
    Q_UNUSED(seek)
}

void PlaybackControl::mediaTypeChanged(MediaCenter::MediaType newType)
{
    Q_UNUSED(newType)
}

void PlaybackControl::setMediaObject(Phonon::MediaObject *mediaObject)
{
    d->mediaObject = mediaObject;
    receivedMediaObject();
}

Phonon::MediaObject* PlaybackControl::mediaObject()
{
    return d->mediaObject;
}

void PlaybackControl::receivedMediaObject()
{}
