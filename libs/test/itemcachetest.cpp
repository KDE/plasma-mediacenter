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

#include "itemcachetest.h"

#include <mediacenter/itemcache.h>
#include <mediacenter/artist.h>

#include <qtest_kde.h>

QTEST_KDEMAIN(ItemCacheTest, NoGUI);

void ItemCacheTest::shouldInsertAndReturnTheSameArtistNextTime()
{
    ItemCache cache;

    auto artist = cache.getArtistByName("Shaan");
    QCOMPARE(artist, cache.getArtistByName("Shaan"));
}

void ItemCacheTest::shouldInsertAndReturnTheSameAlbumNextTime()
{
    ItemCache cache;

    auto album = cache.getAlbumByName("Ghost Stories", "Coldplay");
    QCOMPARE(album, cache.getAlbumByName("Ghost Stories", "Coldplay"));
}

void ItemCacheTest::shouldCreateArtistIfNotPresentWhenCreatingAlbum()
{
    ItemCache cache;

    auto album = cache.getAlbumByName("Ghost Stories", "Coldplay");
    auto artist = cache.getArtistByName("Coldplay", false);
    QVERIFY2(!artist.isNull(), "Artist should be created when creating album");
}

#include "itemcachetest.moc"
