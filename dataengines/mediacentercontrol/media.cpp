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
#include "kdebug.h"

Media::Media()
{
    m_playPause = true;
    m_stop = false;
    m_progress = 0;
    m_state = Paused;

}

QString Media::name() const
{
    return m_name;
}

Media::State Media::state()
{
    return m_state;
}

int Media::length()
{
    return m_length;
}

int Media::position()
{
    return m_progress;
}

qreal Media::volume()
{
    return m_volume;
}

bool Media::canPlay()
{
    if (!m_playPause)
        return true;
    else 
        return false;
}

void Media::play()
{
    m_playPause = true;
    m_state = Playing;
    kDebug() << "play";
}

bool Media::canPause()
{
    if (m_playPause)
        return true;
    else
        return false;
}

void Media::pause()
{
        m_playPause = true;
        m_state = Paused;
        kDebug() << "pause";
}

bool Media::canStop()
{
    if (!m_stop)
        return true;
    else
        return false;
}

void Media::stop()
{
    m_stop = true;
    m_state = Stopped;
    kDebug() << "stop";
}

bool Media::canGoPrevious()
{
    //kDebug() << "yes";
    return true;
}

void Media::previous()
{
    kDebug() << "previous";
}

bool Media::canGoNext()
{
   // kDebug() << "yes";
    return true;
}

void Media::next()
{
    kDebug() << "next";
}

bool Media::canSetVolume()
{
    //kDebug() << "yes";
    return true;
}

void Media::setVolume(qreal volume)
{
    m_volume = volume;
    kDebug() << "volumeSet";
}

bool Media::canSeek()
{
    if (!m_stop)
        return true;
    else
        return false;
}

void Media::seek(int time)
{
    m_progress = time;
    kDebug() << "seek";
}

void Media::setName(const QString& name)
{
    m_name = name;
}

#include "media.moc"