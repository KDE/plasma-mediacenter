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

#include "pmcmediatest.h"
#include "testhelpers.h"

#include <mediacenter/pmcmedia.h>

#include <qtest_kde.h>

QTEST_KDEMAIN(PmcMediaTest, NoGUI);

const QString testUrl = "file:///tmp/test.foo";

void PmcMediaTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void PmcMediaTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void PmcMediaTest::init()
{
    // Called before each testfunction is executed
}

void PmcMediaTest::cleanup()
{
    // Called after every testfunction
}

void PmcMediaTest::shouldReturnUrlEvenIfMediaIsNotSet()
{
    PmcMedia p(testUrl);

    QCOMPARE(testUrl, p.url());
}

void PmcMediaTest::shouldReturnTitleEvenIfMediaIsNotSet()
{
    PmcMedia p(testUrl);

    QCOMPARE(QString("test.foo"), p.title());
}

//TODO: To be enabled after figuring out how to link to Media without exporting
//      it to the public library

// void PmcMediaTest::shouldEmitUpdatedWhenMediaSet()
// {
//     Media m(testUrl);
//     PmcMedia p(testUrl);
//
//     QSignalSpy updatedSpy(&p, SIGNAL(updated()));
//     QVERIFY2(updatedSpy.isValid(), "Could not listen to signal updated");
//
//     p.setMedia(QSharedPointer<Media>(&m));
//
//     waitForSignal(&updatedSpy);
//     QCOMPARE(updatedSpy.size(), 1);
// }

#include "pmcmediatest.moc"
