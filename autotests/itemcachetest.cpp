/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2014 Bhushan Shah <bhush94@gmail.com>
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

#include <itemcache.h>
#include <artist.h>

#include <qtest.h>

QTEST_GUILESS_MAIN(ItemCacheTest);

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

void ItemCacheTest::shouldNotCreateNewArtistIfNotPresent()
{
    ItemCache cache;

    auto artist = cache.getArtistByName("Coldplay", false);
    QVERIFY2(artist.isNull(), "Artist should not be created as create is set to false");
}

void ItemCacheTest::shouldNotCreateNewAlbumOrArtistIfNotPresent()
{
    ItemCache cache;
    auto album = cache.getAlbumByName("Ghost Stories", "Coldplay", false);
    auto artist = cache.getArtistByName("Coldplay", false);

    QVERIFY2(album.isNull(), "Album should not be created here");
    QVERIFY2(artist.isNull(), "Artist should not be created here");
}

#include "itemcachetest.moc"
