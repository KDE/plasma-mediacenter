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

#include <qtest.h>

QTEST_MAIN(ItemCacheTest);

void ItemCacheTest::shouldInsertAndReturnTheSameObjectNextTime()
{
    ItemCache<Artist> artists;

    auto artist = artists.getById("Shaan", true);

    QCOMPARE(artist, artists.getById("Shaan"));
    QCOMPARE(artist, artists.getById("Shaan", true));
}

void ItemCacheTest::shouldNotInsertWhenNotAskedTo()
{
    ItemCache<Artist> artists;

    auto artist = artists.getById("Shaan");

    QVERIFY2(artist.isNull(), "Was expecting a null artist");
}

#include "itemcachetest.moc"
