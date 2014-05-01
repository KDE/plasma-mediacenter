/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "media.h"

#include "artist.h"
#include "album.h"

Album::Album(const QString& name)
    : m_name(name)
{
}

const QString& Album::name() const
{
    return m_name;
}

QString Album::albumArtist() const
{
    //Return the first non-empty artist name
    Q_FOREACH (const QSharedPointer<Media> &m, m_media) {
        const QString artistName = m->artist()->name();
        if (!artistName.isEmpty()) {
            return artistName;
        }
    }

    return QString();
}

void Album::addMedia(const QSharedPointer<Media>& media)
{
    m_media.append(media);
}

void Album::removeMedia(const QSharedPointer<Media>& media)
{
    m_media.removeOne(media);
}

QList< QSharedPointer<Media> > Album::media() const
{
    return m_media;
}
