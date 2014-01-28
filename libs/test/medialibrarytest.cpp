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

#include "medialibrarytest.moc"
