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

#include "pmcalbumtest.h"
#include <mediacenter/album.h>
#include <mediacenter/pmcalbum.h>
#include <mediacenter/media.h>
#include <mediacenter/artist.h>

#include <qtest.h>

QTEST_MAIN(PmcAlbumTest);

void PmcAlbumTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void PmcAlbumTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void PmcAlbumTest::init()
{
    // Called before each testfunction is executed
}

void PmcAlbumTest::cleanup()
{
    // Called after every testfunction
}

void PmcAlbumTest::shouldReturnAlbumArtist()
{
    auto album = QSharedPointer<Album>(new Album("Album"));
    auto media = QSharedPointer<Media>(new Media("Media"));
    auto artist = QSharedPointer<Artist>(new Artist("Artist"));
    media->setArtistAndUpdateRelations(media, artist);

    album->addMedia(media);

    PmcAlbum pmcAlbum(album);

    QCOMPARE(pmcAlbum.albumArtist(), QString("Artist"));
}

void PmcAlbumTest::shouldReturnNameOfAlbum()
{
    auto album = QSharedPointer<Album>(new Album("Album"));
    PmcAlbum pmcAlbum(album);

    QCOMPARE(pmcAlbum.name(), QString("Album"));
}

#include "pmcalbumtest.moc"
