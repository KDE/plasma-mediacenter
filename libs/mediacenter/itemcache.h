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

#ifndef ITEMCACHE_H
#define ITEMCACHE_H

#include <QSharedPointer>
#include <QHash>

class Album;
class Artist;

class ItemCache
{
public:
    QSharedPointer<Artist> getArtistByName(const QString &name, bool create = true);
    QSharedPointer<Album> getAlbumByName(const QString &albumName,
                                         const QString &artistName,
                                         bool create = true);
private:
    QHash<QString, QSharedPointer<Artist>> m_artists;
    QHash<QPair<QString,QString>, QSharedPointer<Album>> m_albums;
};

#endif // ITEMCACHE_H
