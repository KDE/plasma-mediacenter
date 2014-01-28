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

#include <mediacenter/medialibrary.h>

#include <qtest_kde.h>
#include <mediacenter/mediacenter.h>

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
    QDir::current().remove("plasma-mediacenter.db");
}

void MediaLibraryTest::cleanup()
{
    // Called after every testfunction
}

//from https://bugreports.qt-project.org/browse/QTBUG-2986
bool MediaLibraryTest::waitForSignal(QSignalSpy* spy, int timeoutms)
{
    QTime timer;
    timer.start();
    while (spy->isEmpty() && timer.elapsed() < timeoutms)
    {
        QCoreApplication::processEvents();
    }
    return !spy->isEmpty();
}

void MediaLibraryTest::createsDbWhenNotPresent()
{
    MediaLibrary mediaLibrary;

    QSignalSpy initializedSpy(&mediaLibrary, SIGNAL(initialized()));
    QVERIFY2(initializedSpy.isValid(), "Could not listen to signal initialized");

    mediaLibrary.start();
    waitForSignal(&initializedSpy, 2000);

    QVERIFY2(initializedSpy.size() == 1, "MediaLibrary did not emit initialized exactly 1 time");
    QVERIFY2(QDir::current().exists("plasma-mediacenter.db"), "The DB was not created");
}

void MediaLibraryTest::addsNewMedia()
{
    MediaLibrary mediaLibrary;

    QSignalSpy newMediaSpy(&mediaLibrary, SIGNAL(newMedia(QList< QSharedPointer<PmcMedia> >)));
    QVERIFY2(newMediaSpy.isValid(), "Could not listen to signal newMedia");

    mediaLibrary.start();

    QHash<int,QVariant> data;
    data.insert(MediaCenter::MediaUrlRole, "/foo/bar");
    data.insert(Qt::DisplayRole, "Title");
    data.insert(MediaCenter::MediaTypeRole, "audio");
    data.insert(Qt::DecorationRole, "smiley");
    data.insert(MediaCenter::AlbumRole, "album");
    data.insert(MediaCenter::ArtistRole, "artist");

    mediaLibrary.updateMedia(data);

    waitForSignal(&newMediaSpy, 2000);

    QCOMPARE(newMediaSpy.size(), 1);
    QList<QSharedPointer<PmcMedia> > returned = newMediaSpy.takeFirst().first().value< QList<QSharedPointer<PmcMedia> > >();
    QCOMPARE(returned.size(), 1);

    QSharedPointer<PmcMedia> media = returned.first();
    QCOMPARE(media->url(), data.value(MediaCenter::MediaUrlRole).toString());
    QCOMPARE(media->title(), data.value(Qt::DisplayRole).toString());
    QCOMPARE(media->type(), data.value(MediaCenter::MediaTypeRole).toString());
    QCOMPARE(media->thumbnail(), data.value(Qt::DecorationRole).toString());
    QCOMPARE(media->album(), data.value(MediaCenter::AlbumRole).toString());
    QCOMPARE(media->artist(), data.value(MediaCenter::ArtistRole).toString());
}

#include "medialibrarytest.moc"
