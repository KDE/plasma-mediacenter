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
#include "pmccoverartprovider.h"

#include <taglib/tag.h>
#include <taglib/tpropertymap.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/mpegfile.h>

#include <klocalizedstring.h>

#include <QtCore/QMutex>
#include <QtCore/QHash>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QFileInfo>

#include <QDebug>

static const int s_updateInterval = 500;

class MediaInfoService::Private
{
public:
    Private() : requestCounter(0) { }
    quint64 requestCounter; QMutex requestCounterMutex;
    QList<quint64> requestQueue; QMutex requestQueueMutex;
    QHash<quint64, MediaInfoRequest*> requestHash; QMutex requestHashMutex;
    QTimer updateTimer;
};

MediaInfoService::MediaInfoService(QObject* parent)
    : QThread(parent)
    , d(new Private)
{
    moveToThread(this);
    connect(&d->updateTimer, SIGNAL(timeout()), SLOT(processPendingRequests()));
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
    d->updateTimer.start(s_updateInterval);
    exec();
}

void MediaInfoService::processPendingRequests()
{
    Q_ASSERT(thread() == this);

    if (areThereResultsToProcess()) {
        fetchDataForRequest(nextRequestToProcess());
        QTimer::singleShot(0, this, SLOT(processPendingRequests()));
    }
}

bool MediaInfoService::areThereResultsToProcess() const
{
    bool returnValue = false;

    if (d->requestQueueMutex.tryLock(10)) {
        returnValue = !d->requestQueue.isEmpty();
        d->requestQueueMutex.unlock();
    }

    return returnValue;
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
            case MediaInfoRequest::Album:
                result.addData(field, f.tag() && !f.tag()->album().isEmpty() ?
                    f.tag()->album().toCString() : i18n("Unknown album"));
                break;
            default:
                qWarning() << "MediaInfoService does not know about " << field;
            }
        }
    }

    updateAlbumCoverWithCoverArtProvider(result, f);
    emit info(requestNumber, result);
    delete request;
}

void MediaInfoService::updateAlbumCoverWithCoverArtProvider(const MediaInfoResult &result, TagLib::FileRef f) const
{
    if (!f.isNull() && result.availableFields().contains(MediaInfoRequest::Album)) {

        const QString albumName = result.data(MediaInfoRequest::Album).toString();
        if (!PmcCoverArtProvider::containsAlbum(albumName)) {
            TagLib::MPEG::File *mpegFile = dynamic_cast<TagLib::MPEG::File*>(f.file());
            if (!mpegFile)
                return;

            TagLib::ID3v2::Tag *tag = mpegFile->ID3v2Tag();
            if (!tag)
                return;

            TagLib::ID3v2::FrameList l = tag->frameList("APIC");
            if(l.isEmpty())
                return;

            QImage image;
            TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
            image.loadFromData((const uchar *) frame->picture().data(), frame->picture().size());

            PmcCoverArtProvider::addCoverArtImage(albumName, image);
        }
    }
}

quint64 MediaInfoService::nextRequestToProcess() const
{
    QMutexLocker locker(&d->requestQueueMutex);
    return d->requestQueue.takeFirst();
}

#include "mediainfoservice.moc"
