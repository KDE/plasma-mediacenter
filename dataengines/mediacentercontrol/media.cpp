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

}

QString Media::name() const
{
    Q_ASSERT(!m_name.isEmpty());
    return m_name;
}

Media::State Media::state()
{
    m_state = Playing;
    return m_state;
}

int Media::length()
{
    return 60;
}

int Media::position()
{
    return 20;
}

float Media::volume()
{
    return 24;
}

bool Media::canPlay()
{
    kDebug() << "yes";
    return true;
}

void Media::play()
{
    kDebug() << "play";
}

bool Media::canPause()
{
    kDebug() << "yes";
    return true;
}

void Media::pause()
{
    kDebug() <<" Pause";
}

bool Media::canStop()
{
    kDebug() << "yes";
    return true;
}

void Media::stop()
{
    kDebug() << "stop";
}

bool Media::canGoPrevious()
{
    kDebug() << "yes";
    return true;
}

void Media::previous()
{
    kDebug() << "previous";
}

bool Media::canGoNext()
{
    kDebug() << "yes";
    return true;
}

void Media::next()
{
    kDebug() << "next";
}

bool Media::canSetVolume()
{
    kDebug() << "yes";
    return true;
}

void Media::setVolume(qreal)
{
    kDebug() << "volumeSet";
}

bool Media::canSeek()
{
    kDebug() << "yes";
    return true;
}

void Media::seek(int)
{
    kDebug() << "seek";
}

void Media::setName(const QString& name)
{
    m_name = name;
}

#include "media.moc"