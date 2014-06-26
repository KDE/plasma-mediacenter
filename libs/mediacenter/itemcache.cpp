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

#include "itemcache.h"
#include "artist.h"
#include "album.h"

QSharedPointer< Album > ItemCache::getAlbumByName(const QString& albumName,
                                                  const QString& artistName,
                                                  bool create)
{
    auto albumArtistPair = QPair<QString,QString>(albumName, artistName);

    if (!m_albums.contains(albumArtistPair) && create) {
        m_albums.insert(QPair<QString,QString>(albumName, artistName),
                        QSharedPointer<Album>(new Album(albumName, getArtistByName(artistName))));
    }

    return m_albums.value(albumArtistPair);
}

QSharedPointer< Artist > ItemCache::getArtistByName(const QString& name, bool create)
{
    if (!m_artists.contains(name) && create) {
        m_artists.insert(name, QSharedPointer<Artist>(new Artist(name)));
    }

    return m_artists.value(name);
}
