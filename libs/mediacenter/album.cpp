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

#include "album.h"

#include "artist.h"

Album::Album(const Album::AlbumAndArtistNames& albumAndArtistNames)
{
    Q_ASSERT(!albumAndArtistNames.albumName.isEmpty());
    Q_ASSERT(!albumAndArtistNames.artistName.isEmpty());

    m_albumAndArtistNames = albumAndArtistNames;
}

Album::Album()
{
}

Album::~Album()
{
}

const QString& Album::name() const
{
    return m_albumAndArtistNames.albumName;
}

const QSharedPointer< Artist > Album::albumArtist() const
{
    return m_albumArtist;
}

bool Album::setAlbumArtist(const QSharedPointer< Artist >& artist)
{
    if (artist && artist->name() != m_albumAndArtistNames.artistName) {
        m_albumArtist = artist;
        m_albumAndArtistNames.artistName = artist->name();
        return true;
    }

    return false;
}

bool operator<(const Album::AlbumAndArtistNames &lhs,
                              const Album::AlbumAndArtistNames &rhs)
{
    return (lhs.albumName < rhs.albumName) && (lhs.artistName < rhs.artistName);
}
