/*
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "video.h"

#include <QImage>
#include <QPixmap>

#include <KServiceTypeTrader>
#include <KService>
#include <KPixmapCache>
#include <kio/job.h>
#include <kdebug.h>

Video::Video(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args),
m_currentProvider(0),
m_thumbnailsCache(new KPixmapCache("video-thumbnails"))
{}

Video::~Video()
{
    delete m_thumbnailsCache;
}

bool Video::sourceRequestEvent(const QString &source)
{
    qDebug() << "Hello I am the video provider";
    if (source == "providers") {
        KService::List offers = KServiceTypeTrader::self()->query("Plasma/MediaCenter/VideoProvider");
        QStringList providers;
        foreach (KService::Ptr service, offers) {
            providers << service->property("X-KDE-VideoProvider-Name", QVariant::String).toString();
        }
        setData(source, providers);
        return true;
    }

    if (!source.contains(':')) {
        return false;
    }

    const QStringList parms = source.split(':');
    const QString serviceName = parms[0];

    if (!m_currentProvider || m_currentProvider->serviceId() != serviceName) {
        delete m_currentProvider;
        m_currentProvider = 0;
        KService::List offers = KServiceTypeTrader::self()->query("Plasma/MediaCenter/VideoProvider");
        KService::Ptr service;
        KService::List::const_iterator it = offers.constBegin();
        KService::List::const_iterator end = offers.constEnd();
        for (; it != end; ++it) {
            if ((*it)->property("X-KDE-VideoProvider-Name", QVariant::String).toString() == serviceName) {
                service = *it;
                break;
            }
        }

        if (service.isNull()) {
            return false;
        }

        m_currentProvider = service->createInstance<VideoProvider>(this);
        connect(m_currentProvider, SIGNAL(searchResult(QString, QList<VideoPackage>)),
                this, SLOT(dataFromResults(QString, QList<VideoPackage>)));
    }

    QStringList args = parms[1].split('&');
    QString query = args.takeFirst();
    QVariantList constraints;
    foreach (const QString &arg, args) {
        QStringList pieces = arg.split('=');
        if (pieces.count() != 2) {
            continue;
        }
        constraints << pieces[0] << pieces[1];
    }
    setData(parms[1], Plasma::DataEngine::Data());

    m_currentProvider->query(query, constraints);

    return true;
}

void Video::dataFromResults(const QString &query, const QList<VideoPackage> &videos)
{
    QStringList ids;
    QList<VideoPackage>::const_iterator it = videos.constBegin();
    QList<VideoPackage>::const_iterator end= videos.constEnd();

    for (; it != end; ++it) {
        ids << it->id;

        Plasma::DataEngine::Data videoData;
        videoData["author"] = it->author;
        videoData["id"] = it->id;
        videoData["title"] = it->title;
        videoData["description"] = it->description;
//         videoData["thumbnail"] = it->thumbnail;
        KIO::TransferJob *job = KIO::get(KUrl(it->thumbnail), KIO::NoReload, KIO::HideProgressInfo);
        connect (job, SIGNAL(data(KIO::Job*, QByteArray)), this, SLOT(thumbnailReceived(KIO::Job*,QByteArray)));
        m_jobs[job] = it->id;

        videoData["keywords"] = it->keywords;
        videoData["duration"] = it->duration;
        videoData["category"] = it->category;
        videoData["embeddedHTML"] = it->embeddedHTML;

        setData(it->id, videoData);
    }

    // this should never happen since we should keep
    // connection between query and result
    // so query must be the same as the named source
    // from which the query started.
    Q_ASSERT(sources().contains(query));

    setData(query, ids);
}

bool Video::updateSourceEvent(const QString &source)
{
    QPixmap thumbnail;
    if (!m_thumbnailsCache->find(source, thumbnail)) {
        return false;
    }

    setData(source, "thumbnail", thumbnail);
    return true;
}

void Video::thumbnailReceived(KIO::Job *job, const QByteArray &data)
{
    if (!m_jobs.contains(job)) {
        return;
    }

    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data));
    m_thumbnailsCache->insert(m_jobs[job], pixmap);

    updateSourceEvent(m_jobs[job]);
    m_jobs.remove(job);
}

K_EXPORT_PLASMA_DATAENGINE(video, Video)
