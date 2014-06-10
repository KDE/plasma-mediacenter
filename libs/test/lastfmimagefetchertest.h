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

#ifndef LASTFMIMAGEFETCHERTEST_H
#define LASTFMIMAGEFETCHERTEST_H

#include <QObject>

namespace {
    static const int TIMEOUT_FOR_SIGNALS = 10000;
}

class QSignalSpy;

class LastFmImageFetcherTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

//TODO: Enable back when ws.audioscrobbler.com is back online
//      or when we can stub the network call
//     void shouldDownloadImageAndSaveToCache();

private:
    bool waitForSignal(QSignalSpy *spy, int timeout);

};

#endif // LASTFMIMAGEFETCHERTEST_H
