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
#include "testhelpers.h"

#include <medialibrary.h>
#include <kmediacollection.h>

#include <pmcmedia.h>
#include <pmcalbum.h>
#include <pmcartist.h>

#include <qtest.h>

void MediaLibraryTest::initTestCase()
{
}

void MediaLibraryTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void MediaLibraryTest::init()
{
}

void MediaLibraryTest::cleanup()
{
}

QHash<int,QVariant> MediaLibraryTest::createTestMediaData() const
{
    QHash<int,QVariant> data;
    data.insert(KMediaCollection::MediaUrlRole, "/foo/bar");
    data.insert(Qt::DisplayRole, "Title");
    data.insert(KMediaCollection::MediaTypeRole, "audio");
    data.insert(Qt::DecorationRole, "smiley");
    data.insert(KMediaCollection::DurationRole, 100);
    data.insert(KMediaCollection::CreatedAtRole, QDateTime::currentDateTimeUtc());

    return data;
}

QHash< int, QVariant > MediaLibraryTest::createTestMediaDataWithAlbumArtist(const QString &albumName, const QString &artistName, const QString &albumArtistName) const
{
    QHash<int,QVariant> data = createTestMediaData();
    data.insert(KMediaCollection::AlbumRole, albumName);
    data.insert(KMediaCollection::ArtistRole, artistName);
    data.insert(KMediaCollection::AlbumArtistRole, albumArtistName);

    return data;
}

QHash< int, QVariant > MediaLibraryTest::createTestMediaDataWithUrl(const QString &url) const
{
    QHash<int,QVariant> data;
    data.insert(KMediaCollection::MediaUrlRole, url);
    data.insert(Qt::DisplayRole, "Title");
    data.insert(KMediaCollection::MediaTypeRole, "audio");
    data.insert(Qt::DecorationRole, "smiley");
    data.insert(KMediaCollection::DurationRole, 100);
    data.insert(KMediaCollection::CreatedAtRole, QDateTime::currentDateTimeUtc());

    return data;
}

void MediaLibraryTest::addsNewMediaAndItsMetadata()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
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
    QCOMPARE(media->url(), data.value(KMediaCollection::MediaUrlRole).toString());
    QCOMPARE(media->title(), data.value(Qt::DisplayRole).toString());
    QCOMPARE(media->type(), data.value(KMediaCollection::MediaTypeRole).toString());
    QCOMPARE(media->thumbnail(), data.value(Qt::DecorationRole).toString());
    QCOMPARE(media->album(), data.value(KMediaCollection::AlbumRole).toString());
    QCOMPARE(media->artist(), data.value(KMediaCollection::ArtistRole).toString());
    QCOMPARE(media->duration(), data.value(KMediaCollection::DurationRole).toInt());
    QCOMPARE(media->createdAt(), data.value(KMediaCollection::CreatedAtRole).toDateTime());

    QCOMPARE(newAlbumSpy.size(), 1);
    QList<QSharedPointer<PmcAlbum> > returnedAlbum = newAlbumSpy.takeFirst().first().value< QList<QSharedPointer<PmcAlbum> > >();
    QCOMPARE(returnedAlbum.size(), 1);

    QSharedPointer<PmcAlbum> album = returnedAlbum.first();
    QCOMPARE(album->name(), data.value(KMediaCollection::AlbumRole).toString());
    QCOMPARE(album->albumArtist(), data.value(KMediaCollection::AlbumArtistRole).toString());


    QCOMPARE(newArtistSpy.size(), 1);
    QList<QSharedPointer<PmcArtist> > returnedArtist = newArtistSpy.takeFirst().first().value< QList<QSharedPointer<PmcArtist> > >();
    QCOMPARE(returnedArtist.size(), 1);

    QSharedPointer<PmcArtist> artist = returnedArtist.first();
    QCOMPARE(artist->name(), data.value(KMediaCollection::ArtistRole).toString());
}

void MediaLibraryTest::shouldEmitMediaRemovedWhenMediaIsPresentAndRemoved()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
    mediaLibrary.start();

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist();
    QString url = data.value(KMediaCollection::MediaUrlRole).toString();
    mediaLibrary.updateMedia(data);

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 1);

    QSignalSpy mediaRemovedSpy(&mediaLibrary, SIGNAL(mediaRemoved(QSharedPointer<PmcMedia>)));
    QVERIFY2(mediaRemovedSpy.isValid(), "Invalid signal mediaRemoved");
    mediaLibrary.removeMedia(url);

    waitForSignal(&mediaRemovedSpy);

    QCOMPARE(mediaRemovedSpy.count(), 1);
}

void MediaLibraryTest::shouldNotEmitMediaRemovedWhenRemoveMediaIsCalledAndMediaNotPresent()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
    mediaLibrary.start();

    QSignalSpy mediaRemovedSpy(&mediaLibrary, SIGNAL(mediaRemoved(QSharedPointer<PmcMedia>)));
    QVERIFY2(mediaRemovedSpy.isValid(), "Invalid signal newMedia");
    mediaLibrary.removeMedia("/media/not/present/");

    QCOMPARE(mediaRemovedSpy.count(), 0);
}

void MediaLibraryTest::shouldEmitUpdatedForMediaInsteadOfNewMediaWhenDataUpdated()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
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
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
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
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
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

    data.insert(KMediaCollection::AlbumRole, "another_album");
    mediaLibrary.updateMedia(data);

    //Should not emit newMedia
    waitForSignal(&newMediaSpy);
    QCOMPARE(newMediaSpy.size(), 0);

    //Should emit updated for the PmcMedia
    QCOMPARE(mediaUpdatedSpy.size(), 1);
    mediaUpdatedSpy.clear();

    data.insert(KMediaCollection::ArtistRole, "another_artist");
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
    MediaLibrary *mediaLibrary = new MediaLibrary();
    mediaLibrary->start();
    mediaLibrary->setTestMode(true);

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

void MediaLibraryTest::shouldAddDifferentAlbumsWhenArtistsAreDifferent()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
    mediaLibrary.start();

    QSignalSpy newAlbumSpy(&mediaLibrary, SIGNAL(newAlbums(QList< QSharedPointer<PmcAlbum> >)));
    QVERIFY2(newAlbumSpy.isValid(), "Could not listen to signal newAlbums");
    QSignalSpy newArtistSpy(&mediaLibrary, SIGNAL(newArtists(QList< QSharedPointer<PmcArtist> >)));
    QVERIFY2(newArtistSpy.isValid(), "Could not listen to signal newArtists");

    QHash<int,QVariant> data1 = createTestMediaDataWithAlbumArtist();
    QHash<int,QVariant> data2 = createTestMediaDataWithAlbumArtist(data1.value(KMediaCollection::AlbumRole).toString(), "myartist", "awesomeartist");

    mediaLibrary.updateMedia(data1);
    mediaLibrary.updateMedia(data2);

    waitForSignal(&newAlbumSpy);
    waitForSignal(&newArtistSpy);

    QCOMPARE(newAlbumSpy.size(), 1);
    QList<QSharedPointer<PmcAlbum> > returnedAlbums = newAlbumSpy.takeFirst().first().value< QList<QSharedPointer<PmcAlbum> > >();
    QCOMPARE(returnedAlbums.size(), 2);

    QSharedPointer<PmcAlbum> album1 = returnedAlbums.first();
    QCOMPARE(album1->name(), data1.value(KMediaCollection::AlbumRole).toString());
    QCOMPARE(album1->albumArtist(), data1.value(KMediaCollection::AlbumArtistRole).toString());

    QSharedPointer<PmcAlbum> album2 = returnedAlbums.at(1);
    QCOMPARE(album2->name(), data2.value(KMediaCollection::AlbumRole).toString());
    QCOMPARE(album2->albumArtist(), data2.value(KMediaCollection::AlbumArtistRole).toString());


    QCOMPARE(newArtistSpy.size(), 1);
    QList<QSharedPointer<PmcArtist> > returnedArtist = newArtistSpy.takeFirst().first().value< QList<QSharedPointer<PmcArtist> > >();
    QCOMPARE(returnedArtist.size(), 2);

    QSharedPointer<PmcArtist> artist1 = returnedArtist.first();
    QCOMPARE(artist1->name(), data1.value(KMediaCollection::ArtistRole).toString());

    QSharedPointer<PmcArtist> artist2 = returnedArtist.at(1);
    QCOMPARE(artist2->name(), data2.value(KMediaCollection::ArtistRole).toString());
}

void MediaLibraryTest::shouldReturnCorrectAlbumsAndArtists()
{
    MediaLibrary mediaLibrary;
    mediaLibrary.setTestMode(true);
    mediaLibrary.start();

    QSignalSpy initializedSpy(&mediaLibrary, SIGNAL(initialized()));
    QVERIFY2(initializedSpy.isValid(), "Could not listen to signal initialized");

    waitForSignal(&initializedSpy);

    // Initial  state.

    QList< QSharedPointer< PmcAlbum > > initialStateOfAlbums = mediaLibrary.getAlbums();
    QList< QSharedPointer< PmcArtist > > initialStateOfArtists = mediaLibrary.getArtists();

    QVERIFY( initialStateOfAlbums.isEmpty() );
    QVERIFY( initialStateOfArtists.isEmpty() );

    // After inserting 1 media with album and artist info.

    QSignalSpy newAlbumSpy(&mediaLibrary, SIGNAL(newAlbums(QList< QSharedPointer<PmcAlbum> >)));
    QVERIFY2(newAlbumSpy.isValid(), "Could not listen to signal newAlbums");
    QSignalSpy newArtistSpy(&mediaLibrary, SIGNAL(newArtists(QList< QSharedPointer<PmcArtist> >)));
    QVERIFY2(newArtistSpy.isValid(), "Could not listen to signal newArtists");

    QHash<int,QVariant> data = createTestMediaDataWithAlbumArtist("Album for test", "Artist for test");
    mediaLibrary.updateMedia(data);

    waitForSignal(&newAlbumSpy);
    waitForSignal(&newArtistSpy);

    QList< QSharedPointer< PmcAlbum > > stateOfAlbums = mediaLibrary.getAlbums();
    QList< QSharedPointer< PmcArtist > > stateOfArtists = mediaLibrary.getArtists();

    QCOMPARE(stateOfAlbums.count(), 1);
    QCOMPARE(stateOfAlbums.at(0)->name(), QString("Album for test"));
    QCOMPARE(stateOfArtists.count(), 1);
    QCOMPARE(stateOfArtists.at(0)->name(), QString("Artist for test"));

    // After inserting another media with different artist and without album info.

    QSignalSpy newAlbumSpy2(&mediaLibrary, SIGNAL(newAlbums(QList< QSharedPointer<PmcAlbum> >)));
    QVERIFY2(newAlbumSpy2.isValid(), "Could not listen to signal newAlbums");
    QSignalSpy newArtistSpy2(&mediaLibrary, SIGNAL(newArtists(QList< QSharedPointer<PmcArtist> >)));
    QVERIFY2(newArtistSpy2.isValid(), "Could not listen to signal newArtists");

    QHash<int,QVariant> data2 = createTestMediaDataWithUrl("/bar/foo");
    data2.insert(KMediaCollection::ArtistRole, "Artist");
    mediaLibrary.updateMedia(data2);

    waitForSignal(&newAlbumSpy2);
    waitForSignal(&newArtistSpy2);

    QList< QSharedPointer< PmcAlbum > > stateOfAlbums2 = mediaLibrary.getAlbums();
    QList< QSharedPointer< PmcArtist > > stateOfArtists2 = mediaLibrary.getArtists();

    QCOMPARE(stateOfAlbums2.count(), 1);
    QCOMPARE(stateOfArtists2.count(), 2);

    // After inserting another media with different album and without artist info.

    QSignalSpy newAlbumSpy3(&mediaLibrary, SIGNAL(newAlbums(QList< QSharedPointer<PmcAlbum> >)));
    QVERIFY2(newAlbumSpy3.isValid(), "Could not listen to signal newAlbums");
    QSignalSpy newArtistSpy3(&mediaLibrary, SIGNAL(newArtists(QList< QSharedPointer<PmcArtist> >)));
    QVERIFY2(newArtistSpy3.isValid(), "Could not listen to signal newArtists");

    QHash<int,QVariant> data3 = createTestMediaDataWithUrl("/bar/foo123");
    data3.insert(KMediaCollection::AlbumRole, "Album");
    mediaLibrary.updateMedia(data3);

    waitForSignal(&newAlbumSpy3);
    waitForSignal(&newArtistSpy3);

    QList< QSharedPointer< PmcAlbum > > stateOfAlbums3 = mediaLibrary.getAlbums();
    QList< QSharedPointer< PmcArtist > > stateOfArtists3 = mediaLibrary.getArtists();

    QCOMPARE(stateOfAlbums3.count(), 2);
    QCOMPARE(stateOfArtists3.count(), 2);
}

QTEST_GUILESS_MAIN(MediaLibraryTest)
