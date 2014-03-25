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

#include <QCoreApplication>
#include <QSignalSpy>
#include <QTime>

namespace {
    static const int TIMEOUT_FOR_SIGNALS = 3000;
}

//from https://bugreports.qt-project.org/browse/QTBUG-2986
bool waitForSignal(QSignalSpy* spy, int timeoutms = TIMEOUT_FOR_SIGNALS)
{
    QTime timer;
    timer.start();
    while (spy->isEmpty() && timer.elapsed() < timeoutms)
    {
        QCoreApplication::processEvents();
    }
    return !spy->isEmpty();
}
