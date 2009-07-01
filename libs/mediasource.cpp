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
#include "mediasource.h"

using namespace MediaCenter;

MediaSource::MediaSource(MediaType type, const KUrl &url) :
        m_url(url),
        m_type(type),
        m_deviceName(QString())
{
}

MediaSource::MediaSource(MediiaType type, const QString &deviceName) :
        m_url(KUrl()),
        m_type(type),
        m_deviceName(deviceName)
{
}

MediaSource::~MediaSource()
{}

MediaType MediaSource::type() const
{
    return m_type;
}

KUrl MediaSource::url() const
{
    return m_url;
}

QString MediaSource::deviceName() const
{
    return m_deviceName;
}
