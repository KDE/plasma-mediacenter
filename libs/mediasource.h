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
#ifndef MEDIASOURCE_H
#define MEDIASOURCE_H

#include <mediacenter.h>

#include <KUrl>

namespace MediaCenter {

class MediaSource
{
public:
    /**
     * Builds a media source identifying a specific media
     * by its @param type and its @param url.
     */
    MediaSource(MediaType type, const KUrl& url);

    /**
     * Use this constructor if @param type is either a Cd, Dvd,
     * or Vcd, since probably you won't have a url to pass.
     *
     * @param deviceName is the unique device identifier, recommended to be
     * retrieved via Solid.
     */
    MediaSource(MediaType type, const QString &deviceName);

    ~MediaSource();

    MediaType type() const;
    KUrl url() const;
    QString deviceName() const;

private:
    KUrl m_url;
    MediaType m_type;
    QString m_deviceName;
};
}

#endif // MEDIASOURCE_H
