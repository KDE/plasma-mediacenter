/***************************************************************************
 *   Copyright 2011 by Sinny Kumari <ksinny@gmail.com>                     *
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

#include "media.h"
#include <KDebug>

Media::Media() : m_state(Stopped), m_length(0), m_volume(0.5), m_position(0),
    m_dirty(false), m_viewMode(false)
{
}

QString Media::name() const
{
    return m_name;
}

Media::State Media::state()
{
    return m_state;
}

int Media::length() const
{
    return m_length;
}

void Media::setLength(int time)
{
    m_length = time;
    emit mediaDataUpdated();
}

int Media::position() const
{
    return m_position;
}

qreal Media::volume() const
{
    return m_volume;
}

bool Media::canPlay() const
{
    return (m_state==Paused||m_state==Stopped);
}

void Media::play()
{
    m_state = Playing;
    emit mediaDataUpdated();
}

bool Media::canPause() const
{
    return (m_state==Playing);
}

void Media::pause()
{
    m_state = Paused;
    emit mediaDataUpdated();
}

bool Media::canStop() const
{
    return (m_state==Playing||m_state==Paused);
}

void Media::stop()
{
    m_state = Stopped;
    m_position = 0;
    emit mediaDataUpdated();
}

bool Media::canGoPrevious() const
{
    return true;
}

void Media::previous()
{
}

bool Media::canGoNext() const
{
    return true;
}

void Media::next()
{
}

bool Media::canSetVolume() const
{
    return true;
}

void Media::setVolume(qreal volume)
{
    m_volume = volume;
    emit mediaDataUpdated();
}

bool Media::canSeek() const
{
    return (m_state==Playing||m_state==Paused);
}

void Media::seek(int time)
{
    m_position = time;
    m_dirty = true;
    emit mediaDataUpdated();
}

bool Media::canUpdateMediaProgress() const
{
    return (m_state==Playing);
}

void Media::mediaProgress(int time)
{
    m_position = time;
    emit mediaDataUpdated();
}

void Media::setName(const QString& name)
{
    m_name = name;
}

void Media::setUrl(QString url)
{
    m_length = 0;
    m_position = 0;
    m_url = url;
    m_state = Playing;
    emit mediaDataUpdated();
}

bool Media::getDirty() const
{
    return m_dirty;
}


void Media::setDirty(bool dirtyBit)
{
    m_dirty = dirtyBit;
    emit mediaDataUpdated();
}

QString Media::getUrl() const
{
    return m_url;
}

QString Media::browsingState() const
{
    return m_browsingState;
}

void Media::setViewMode(bool mode)
{
    m_viewMode = mode;
    if (mode == false && m_browsingState == "PictureBrowsing") {
        m_url = "";
    }
    emit mediaDataUpdated();
}

bool Media::viewMode() const
{
    return m_viewMode;
}

void Media::setBrowsingState(const QString& state)
{
    m_browsingState = state;
    emit mediaDataUpdated();
}

QString Media::currentBackendName() const
{
    return m_backendName;
}

void Media::setCurrentBackendName (const QString& backendName)
{
    m_backendName = backendName;
    emit mediaDataUpdated();
}

#include "media.moc"