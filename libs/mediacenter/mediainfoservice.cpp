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

#include "mediainfoservice.h"

#include "mediainforequest.h"
#include "mediainforesult.h"

#include <fileref.h>
#include <tag.h>

#include <QtCore/QMutex>
#include <QtCore/QHash>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include <QDebug>
#include <QFileInfo>
#include <klocalizedstring.h>

class MediaInfoService::Private
{
public:
    quint64 requestCounter; QMutex requestCounterMutex;
    QList<quint64> requestQueue; QMutex requestQueueMutex;
    QHash<quint64, MediaInfoRequest*> requestHash; QMutex requestHashMutex;
};

MediaInfoService::MediaInfoService(QObject* parent)
    : QThread(parent)
    , d(new Private)
{
    moveToThread(this);
}

MediaInfoService::~MediaInfoService()
{
}

quint64 MediaInfoService::processRequest(MediaInfoRequest* request)
{
    d->requestCounterMutex.lock();
    quint64 requestNumber = d->requestCounter++;
    d->requestCounterMutex.unlock();

    d->requestHashMutex.lock();
    d->requestHash.insert(requestNumber, request);
    d->requestHashMutex.unlock();

    d->requestQueueMutex.lock();
    d->requestQueue.append(requestNumber);
    d->requestQueueMutex.unlock();

    return requestNumber;
}

void MediaInfoService::run()
{
    QTimer::singleShot(0, this, SLOT(processPendingRequests()));
    exec();
}

void MediaInfoService::processPendingRequests()
{
    Q_ASSERT(thread() == this);
    if (areThereResultsToProcess()) {
        fetchDataForRequest(nextRequestToProcess());
        QTimer::singleShot(0, this, SLOT(processPendingRequests()));
    } else {
        QTimer::singleShot(0, this, SLOT(processPendingRequests()));
    }
}

bool MediaInfoService::areThereResultsToProcess() const
{
    QMutexLocker locker(&d->requestQueueMutex);
    return !d->requestQueue.isEmpty();
}

void MediaInfoService::fetchDataForRequest(quint64 requestNumber)
{
    d->requestHashMutex.lock();
    MediaInfoRequest *request = d->requestHash.take(requestNumber);
    d->requestHashMutex.unlock();
    if (!request)
        return;

    MediaInfoResult result;
    TagLib::FileRef f(QUrl(request->mediaPath()).toLocalFile().toUtf8().constData());

    while (request->hasFields()) {
        MediaInfoRequest::InformationField field = request->takeField();
        if (f.isNull()) {
            result.addData(field, QVariant());
        } else {
            switch (field) {
            case MediaInfoRequest::Title:
                result.addData(field, f.tag() && !f.tag()->title().isEmpty() ?
                    f.tag()->title().toCString() : QFileInfo(request->mediaPath()).fileName());
                break;
            case MediaInfoRequest::Artist:
                result.addData(field, f.tag() && !f.tag()->artist().isEmpty() ?
                    f.tag()->artist().toCString() : i18n("Unknown artist"));
                break;
            case MediaInfoRequest::Length:
                result.addData(field, f.audioProperties() ? f.audioProperties()->length() : 0);
                break;
            default:
                qWarning() << "MediaInfoService does not know about " << field; 
            }
        }
    }

    emit info(requestNumber, result);
    delete request;
}

quint64 MediaInfoService::nextRequestToProcess() const
{
    QMutexLocker locker(&d->requestQueueMutex);
    return d->requestQueue.takeFirst();
}

#include "mediainfoservice.moc"
