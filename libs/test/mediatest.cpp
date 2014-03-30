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
#include <mediacenter/media.h>

#include <qtest_kde.h>

QTEST_KDEMAIN(MediaTest, NoGUI);

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

void MediaTest::shouldSetDurationToZeroWhenNotSpecified()
{
    Media m;
    QCOMPARE(m.duration(), 0);
}

#include "mediatest.moc"
