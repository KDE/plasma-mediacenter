/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "playbackgesture.h"

#include <mediacenter/mediacenter.h>

using namespace MediaCenter;

PlaybackGesture::PlaybackGesture(QObject *parent) :
    QGesture(parent)
{
}

qreal PlaybackGesture::volumeLevel() const
{
    return m_volumeLevel;
}

PlaybackGesture::ControlAction PlaybackGesture::activeAction() const
{
    return m_activeAction;
}

void PlaybackGesture::increaseVolume(int factor)
{
    qreal realFactor = factor / 10;
    if (m_volumeLevel + realFactor > 1) {
        m_volumeLevel = 1;
        return;
    }

    m_volumeLevel += realFactor;
}

void PlaybackGesture::decreaseVolume(int factor)
{
    qreal realFactor = factor / 10;
    if (m_volumeLevel - realFactor < 0) {
        m_volumeLevel = 0;
        return;
    }

    m_volumeLevel -= realFactor;
}
