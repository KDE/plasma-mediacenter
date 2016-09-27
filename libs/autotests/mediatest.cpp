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

#include "mediatest.h"

#include <album.h>
#include <artist.h>
#include <media.h>

#include <qtest.h>

QTEST_GUILESS_MAIN(MediaTest);

const QString TEST_URL = "uri://test.123";

void MediaTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void MediaTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void MediaTest::init()
{
    // Called before each testfunction is executed
}

void MediaTest::cleanup()
{
    // Called after every testfunction
}

void MediaTest::shouldSetAlbumAndUpdateRelationsCorrectly()
{
    QSharedPointer<Artist> artist(new Artist("Artist"));
    QSharedPointer<Album> oldAlbum(new Album("GNOME", artist));
    QSharedPointer<Album> newAlbum(new Album("KDE", artist));

    QSharedPointer<Media> media = QSharedPointer<Media>(new Media(TEST_URL));
    media->setAlbumAndUpdateRelations(media, oldAlbum);

    QCOMPARE(media->album(), oldAlbum);
    QCOMPARE(oldAlbum->media().at(0), media);

    media->setAlbumAndUpdateRelations(media, newAlbum);
    QCOMPARE(media->album(), newAlbum);
    QCOMPARE(newAlbum->media().at(0), media);
    QCOMPARE(oldAlbum->media().size(), 0);
}

void MediaTest::shouldSetArtistAndUpdateRelationsCorrectly()
{
    QSharedPointer<Artist> oldArtist(new Artist("Foo"));
    QSharedPointer<Artist> newArtist(new Artist("Bar"));

    QSharedPointer<Media> media = QSharedPointer<Media>(new Media(TEST_URL));
    media->setArtistAndUpdateRelations(media, oldArtist);

    QCOMPARE(media->artist(), oldArtist);
    QCOMPARE(oldArtist->media().at(0), media);

    media->setArtistAndUpdateRelations(media, newArtist);
    QCOMPARE(media->artist(), newArtist);
    QCOMPARE(newArtist->media().at(0), media);
    QCOMPARE(oldArtist->media().size(), 0);
}
