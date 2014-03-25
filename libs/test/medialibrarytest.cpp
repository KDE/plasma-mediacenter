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

#include "medialibrarytest.h"
#include "fakemediavalidator.h"
#include "testhelpers.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <mediacenter/pmcmedia.h>
#include <mediacenter/pmcalbum.h>
#include <mediacenter/pmcartist.h>

#include <qtest_kde.h>

static const char* DB_FILENAME = "plasma-mediacenter.sqlite";

QTEST_KDEMAIN(MediaLibraryTest, NoGUI);

void MediaLibraryTest::initTestCase()
{
}

void MediaLibraryTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void MediaLibraryTest::init()
{
    QDir::current().remove(DB_FILENAME);
}

void MediaLibraryTest::cleanup()
{
    QDir::current().remove(DB_FILENAME);
}

QHash<int,QVariant> MediaLibraryTest::createTestMediaData() const
{
    QHash<int,QVariant> data;
    data.insert(MediaCenter::MediaUrlRole, "/foo/bar");
    data.insert(Qt::DisplayRole, "Title");
    data.insert(MediaCenter::MediaTypeRole, "audio");
    data.insert(Qt::DecorationRole, "smiley");

    return data;
}

QHash< int, QVariant > MediaLibraryTest::createTestMediaDataWithAlbumArtist() const
{
    QHash<int,QVariant> data = createTestMediaData();
    data.insert(MediaCenter::AlbumRole, "album");
    data.insert(MediaCenter::ArtistRole, "artist");

    return data;
}

void MediaLibraryTest::createsDbWhenNotPresent()
{
    MediaLibrary mediaLibrary;

    QSignalSpy initializedSpy(&mediaLibrary, SIGNAL(initialized()));
    QVERIFY2(initializedSpy.isValid(), "Could not listen to signal initialized");

    mediaLibrary.start();
    waitForSignal(&initializedSpy);

    QVERIFY2(initializedSpy.size() == 1, "MediaLibrary did not emit initialized exactly 1 time");
    QVERIFY2(QDir::current().exists(DB_FILENAME), "The DB was not created");
}

void MediaLibraryTest::addsNewMediaAndItsAlbumArtist()
{
    FakeMediaValidator validator;
    MediaLibrary mediaLibrary(&validator);
    mediaLibrary.start();

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");
    QSignalSpy newAlbumSpy(&mediaLibrary, SIGNAL(newAlbums(QList< QSharedPointer<PmcAlbum> >)));
    QVERIFY2(newAlbumSpy.isValid(), "Could not listen to signal newAlbums");
    QSignalSpy newArtistSpy(&mediaLibrary, SIGNAL(newArtists(QList< QSharedPointer<PmcArtist> >)));
    QVERIFY2(newArtistSpy.isValid(), "Could not listen to signal newArtists");

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy);
    waitForSignal(&newAlbumSpy);
    waitForSignal(&newArtistSpy);

    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returnedMedia = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returnedMedia.size(), 1);

    QSharedPointer<PmcMedia> media = returnedMedia.first();
    QCOMPARE(media->url(), data.value(MediaCenter::MediaUrlRole).toString());
    QCOMPARE(media->title(), data.value(Qt::DisplayRole).toString());
    QCOMPARE(media->type(), data.value(MediaCenter::MediaTypeRole).toString());
    QCOMPARE(media->thumbnail(), data.value(Qt::DecorationRole).toString());
    QCOMPARE(media->album(), data.value(MediaCenter::AlbumRole).toString());
    QCOMPARE(media->artist(), data.value(MediaCenter::ArtistRole).toString());


    QCOMPARE(newAlbumSpy.size(), 1);
    QList<QSharedPointer<PmcAlbum> > returnedAlbum = newAlbumSpy.takeFirst().first().value< QList<QSharedPointer<PmcAlbum> > >();
    QCOMPARE(returnedAlbum.size(), 1);

    QSharedPointer<PmcAlbum> album = returnedAlbum.first();
    QCOMPARE(album->name(), data.value(MediaCenter::AlbumRole).toString());
    QCOMPARE(album->albumArtist(), data.value(MediaCenter::ArtistRole).toString());


    QCOMPARE(newArtistSpy.size(), 1);
    QList<QSharedPointer<PmcArtist> > returnedArtist = newArtistSpy.takeFirst().first().value< QList<QSharedPointer<PmcArtist> > >();
    QCOMPARE(returnedArtist.size(), 1);

    QSharedPointer<PmcArtist> artist = returnedArtist.first();
    QCOMPARE(artist->name(), data.value(MediaCenter::ArtistRole).toString());
}

void MediaLibraryTest::shouldEmitUpdatedForMediaInsteadOfNewMediaWhenDataUpdated()
{
    FakeMediaValidator validator;
    MediaLibrary mediaLibrary(&validator);
    mediaLibrary.start();

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returnedMedia = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returnedMedia.size(), 1);

    QSharedPointer<PmcMedia> pmcMedia = returnedMedia.at(0);
    QSignalSpy mediaUpdatedSpy(pmcMedia.data(), SIGNAL(updated()));

    data.insert(Qt::DisplayRole, "New title");
    mediaLibrary.updateMedia(data);

    //Should not emit newMedia
    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 0);

    //Should emit updated for the PmcMedia
    QCOMPARE(mediaUpdatedSpy.size(), 1);
}

void MediaLibraryTest::shouldNotEmitUpdatedWhenNothingUpdated()
{
    FakeMediaValidator validator;
    MediaLibrary mediaLibrary(&validator);
    mediaLibrary.start();

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returnedMedia = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returnedMedia.size(), 1);

    QSharedPointer<PmcMedia> pmcMedia = returnedMedia.at(0);
    QSignalSpy mediaUpdatedSpy(pmcMedia.data(), SIGNAL(updated()));

    mediaLibrary.updateMedia(data);

    //Should not emit newMedia
    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 0);

    //Should not emit updated for the PmcMedia
    QCOMPARE(mediaUpdatedSpy.size(), 0);
}

void MediaLibraryTest::shouldEmitUpdatedWhenAlbumOrArtistChanged()
{
    FakeMediaValidator validator;
    MediaLibrary mediaLibrary(&validator);
    mediaLibrary.start();

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returnedMedia = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returnedMedia.size(), 1);

    QSharedPointer<PmcMedia> pmcMedia = returnedMedia.at(0);
    QSignalSpy mediaUpdatedSpy(pmcMedia.data(), SIGNAL(updated()));

    data.insert(MediaCenter::AlbumRole, "another_album");
    mediaLibrary.updateMedia(data);

    //Should not emit newMedia
    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 0);

    //Should emit updated for the PmcMedia
    QCOMPARE(mediaUpdatedSpy.size(), 1);
    mediaUpdatedSpy.clear();

    data.insert(MediaCenter::ArtistRole, "another_artist");
    mediaLibrary.updateMedia(data);

    //Should not emit newMedia
    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 0);

    //Should emit updated for the PmcMedia
    QCOMPARE(mediaUpdatedSpy.size(), 1);
}

void MediaLibraryTest::shouldNotAddMediaForNonExistentFile()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.start();

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy, 6000);

    QCOMPARE(newMediaSpy.size(), 0);
}

void MediaLibraryTest::shouldCleanupEntriesForNonExistentMedia()
{
    FakeMediaValidator validator;
    MediaLibrary *mediaLibrary = new MediaLibrary(&validator);
    mediaLibrary->start();

    QSignalSpy newMediaSpy(mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();

    mediaLibrary->updateMedia(data);

    waitForSignal(&newMediaSpy);

    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returnedMedia = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returnedMedia.size(), 1);

    delete mediaLibrary;

    mediaLibrary = new MediaLibrary();

    QSignalSpy anotherNewMediaSpy(mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(anotherNewMediaSpy.isValid(), "Could not listen to signal newMedia");

    mediaLibrary->start();

    waitForSignal(&anotherNewMediaSpy);
    QCOMPARE(anotherNewMediaSpy.size(), 0);

    delete mediaLibrary;
}

#include "medialibrarytest.moc"
