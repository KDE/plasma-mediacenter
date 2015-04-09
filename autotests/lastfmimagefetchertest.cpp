/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#include "lastfmimagefetchertest.h"
#include "lastfmimagefetcher.h"
#include "pmcimagecache.h"
#include "singletonfactory.h"

#include <qtest.h>

void LastFmImageFetcherTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void LastFmImageFetcherTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void LastFmImageFetcherTest::init()
{
    // Called before each testfunction is executed
}

void LastFmImageFetcherTest::cleanup()
{
    // Called after every testfunction
}

void LastFmImageFetcherTest::shouldDownloadArtistImageAndSaveToCache()
{
    LastFmImageFetcher lastFmFetcher;
    QSignalSpy spyInitialize(&lastFmFetcher, SIGNAL(imageFetched(QVariant,QString)));
    QVERIFY2(spyInitialize.isValid(), "Can't listen to signal imageFetched");
    QSignalSpy spyNetwork(&lastFmFetcher, SIGNAL(serviceUnavailable()));
    QVERIFY2(spyNetwork.isValid(), "Can't listen to signal serviceUnavailable");

    lastFmFetcher.fetchImage("artist", "Myfaveartist", "shaan");

    waitForSignal(&spyNetwork, TIMEOUT_FOR_SIGNALS);

    if(spyNetwork.size() >= 1)
        QSKIP("Network connection unavailable", SkipAll);

    waitForSignal(&spyInitialize, TIMEOUT_FOR_SIGNALS);

    QCOMPARE(spyInitialize.size(), 1);
    QList<QVariant> arguments = spyInitialize.takeFirst();
    QCOMPARE(arguments.at(0).value<QVariant>().toString(), QString("Myfaveartist"));
    QCOMPARE(arguments.at(1).toString(), QString("shaan"));
    QVERIFY2(SingletonFactory::instanceFor<PmcImageCache>()->containsImageWithId("artist:shaan"), "Cache did not contain artist image");
}

void LastFmImageFetcherTest::shouldDownloadAlbumImageAndSaveToCache()
{
    LastFmImageFetcher lastFmFetcher;
    QSignalSpy spyInitialize(&lastFmFetcher, SIGNAL(imageFetched(QVariant,QString)));
    QVERIFY2(spyInitialize.isValid(), "Can't listen to signal imageFetched");
    QSignalSpy spyNetwork(&lastFmFetcher, SIGNAL(serviceUnavailable()));
    QVERIFY2(spyNetwork.isValid(), "Can't listen to signal serviceUnavailable");

    lastFmFetcher.fetchImage("album", "Myfavealbum", "Cher", "Believe");

    waitForSignal(&spyNetwork, TIMEOUT_FOR_SIGNALS);

    if(spyNetwork.size() >= 1)
        QSKIP("Network connection unavailable");

    waitForSignal(&spyInitialize, TIMEOUT_FOR_SIGNALS);

    QCOMPARE(spyInitialize.size(), 1);
    QList<QVariant> arguments = spyInitialize.takeFirst();
    QCOMPARE(arguments.at(0).value<QVariant>().toString(), QString("Myfavealbum"));
    QCOMPARE(arguments.at(1).toString(), QString("Believe"));
    QVERIFY2(SingletonFactory::instanceFor<PmcImageCache>()->containsImageWithId("album:Believe"), "Cache did not contain album image");
}

bool LastFmImageFetcherTest::waitForSignal(QSignalSpy* spy, int timeout)
{
    QTime timer;
    timer.start();
    while (spy->isEmpty() && timer.elapsed() < timeout)
    {
        QCoreApplication::processEvents();
    }
    return !spy->isEmpty();
}

QTEST_GUILESS_MAIN(LastFmImageFetcherTest)

