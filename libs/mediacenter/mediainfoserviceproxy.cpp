/***********************************************************************************
 *   Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                          *
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

#include "mediainfoserviceproxy.h"
#include "mediainfoservice.h"

#include <QtCore/QList>

#include <QDebug>

static const int s_maxThreads = 2;
MediaInfoServiceProxy *MediaInfoServiceProxy::m_instance = 0;

class MediaInfoServiceProxy::Private
{
public:
    Private() { counter = 0; }
    QList<MediaInfoService*> threads;
    int counter;
};

MediaInfoServiceProxy* MediaInfoServiceProxy::instance()
{
    if (!m_instance) {
        m_instance = new MediaInfoServiceProxy();
    }
    return m_instance;
}

MediaInfoServiceProxy::MediaInfoServiceProxy()
    :d(new Private)
{
    for (int i=0; i<s_maxThreads; ++i) {
        MediaInfoService *service = new MediaInfoService;
        d->threads.append(service);
        service->start();
    }
}

MediaInfoServiceProxy::~MediaInfoServiceProxy()
{
    foreach(MediaInfoService *service, d->threads) {
        service->deleteLater();
    }
}

QPair< quint64, MediaInfoService* > MediaInfoServiceProxy::processRequest(MediaInfoRequest* request)
{
    MediaInfoService *thread = d->threads.at(d->counter);
    incrementCounter();

    int requestNumber = thread->processRequest(request);
    return QPair< quint64, MediaInfoService* >(requestNumber, thread);
}

void MediaInfoServiceProxy::incrementCounter()
{
    d->counter++;
    if (d->counter == s_maxThreads) {
        d->counter = 0;
    }
}
