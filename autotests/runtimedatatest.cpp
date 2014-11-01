/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "runtimedatatest.h"

#include "mediacenter/runtimedata.h"

#include <QtTest>

const QString TEST_URL = "uri://test.123";
const QString TEST_URL2 = "uri://test.234";

QTEST_GUILESS_MAIN(TestRuntimeData);

void TestRuntimeData::initTestCase()
{
    // Called before the first testfunction is executed
}

void TestRuntimeData::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void TestRuntimeData::init()
{
    // Called before each testfunction is executed
}

void TestRuntimeData::cleanup()
{
    // Called after every testfunction
}

void TestRuntimeData::testPlayUrl()
{
    RuntimeData runtimeData;
    //First try setting url
    runtimeData.playUrl(TEST_URL);
    QCOMPARE(runtimeData.property("url").toString(), TEST_URL);
    //Now again play different url
    runtimeData.playUrl(TEST_URL2);
    QCOMPARE(runtimeData.property("url").toString(), TEST_URL2);
}

void TestRuntimeData::testVolumeAndMuteFunction()
{
    RuntimeData runtimeData;

    //Lets set volume to something reasonable
    runtimeData.setVolume(5.0);
    QCOMPARE(runtimeData.property("volume").toReal(), 5.0);

    //Now toggle mute and check volume
    runtimeData.toggleMute();
    QCOMPARE(runtimeData.property("volume").toReal(), 0.0);

    //Setting volume to positive value should set mute to false;
    runtimeData.setVolume(5.0);
    QCOMPARE(runtimeData.property("mute").toBool(), false);

    //Setting volume to zero should toggle mute
    runtimeData.setVolume(0.0);
    QCOMPARE(runtimeData.property("mute").toBool(), true);

    //toggling mute should set volume to previous one.
    runtimeData.toggleMute();
    QCOMPARE(runtimeData.property("volume").toReal(), 5.0);
}

#include "runtimedatatest.moc"
