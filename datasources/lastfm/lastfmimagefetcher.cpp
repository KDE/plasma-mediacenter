/***********************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                             *
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

#include "lastfmimagefetcher.h"
#include "mediacenter/pmcimagecache.h"
#include "mediacenter/singletonfactory.h"
#include "mediacenter/medialibrary.h"
#include "mediacenter/pmcmetadatamodel.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDomDocument>
#include <QSharedPointer>

#include <QTimer>
#include <QImage>
#include <QDebug>

MEDIACENTER_EXPORT_DATASOURCE(LastFmImageFetcher, "lastfmimagefetcher.json")

LastFmImageFetcher::LastFmImageFetcher(QObject* parent, const QVariantList& args)
    : AbstractDataSource(parent, args)
    , m_busy(false)
    , m_artistInfoUrl("http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&artist=%1&api_key=22a6906e49bffd8cc11be1385aea73de")
    , m_albumInfoUrl("http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=%1&album=%2&api_key=22a6906e49bffd8cc11be1385aea73de")

{
    connect(SingletonFactory::instanceFor<MediaLibrary>(), &MediaLibrary::newMedia, this, &LastFmImageFetcher::handleLastFmNewMedia);
}

LastFmImageFetcher::~LastFmImageFetcher()
{

}

void LastFmImageFetcher::fetchImage(const QString& type, const QVariant& identifier, const QString& artistName, const QString& albumName)
{
    if (albumList.contains(albumName) || artistList.contains(artistName)) {
        return;
    }
    QStringList nameList;
    nameList << type << artistName;
    if (!albumName.isEmpty()) {
        nameList << albumName;
        albumList.append(albumName);
    }
    if (!artistList.isEmpty()) {
        artistList.append(artistName);
    }
    m_pendingQueue.enqueue(nameList);
    m_identifiers.insert(albumName.isEmpty() ? artistName : albumName, identifier);
    QTimer::singleShot(0, this, SLOT(processQueue()));
}

void LastFmImageFetcher::processQueue()
{
    if (m_pendingQueue.empty()) {
        QTimer::singleShot(1000, this, SLOT(processQueue()));
        return;
    }

    const QStringList nameList = m_pendingQueue.dequeue();

    QUrl apiUrl;
    if (nameList.count() > 2) {
        apiUrl = QUrl(m_albumInfoUrl.arg(nameList.at(1), nameList.at(2)));
    } else {
        apiUrl = QUrl(m_artistInfoUrl.arg(nameList.at(1)));
    }

    // qDebug() << "Fetching " << apiUrl;
    QNetworkReply *reply = m_netAccessManager->get(QNetworkRequest(apiUrl));
    m_currentInfoDownloads.insert(reply,
                                  nameList.count() > 2 ? nameList.at(2) : nameList.at(1));

    m_busy = true;
}

void LastFmImageFetcher::gotResponse(QNetworkReply* reply)
{

    if (reply->error() != QNetworkReply::NoError) {
        //qDebug() << reply->errorString();
        emit serviceUnavailable();
        return;
    }

    QDomDocument doc;
    doc.setContent(reply->readAll());

    const QDomElement firstElement = doc.firstChildElement().firstChildElement();
    const QString type = firstElement.tagName();

    const QString name = m_currentInfoDownloads.take(reply);
    const QDomNodeList imageList = firstElement.childNodes();

    for (int i=imageList.length(); i>0; i--) {
        const QDomElement element = imageList.at(i).toElement();
        if (element.tagName() == "image" &&
            (element.attribute("size") == "extralarge"
                || element.attribute("size") == "large")) {
            downloadImage(type, name, element.text());
            return;
        }
    }

//    qDebug() << "Webservice has no image for " << name;
    QTimer::singleShot(0, this, SLOT(processQueue()));

    reply->deleteLater();
}

void LastFmImageFetcher::downloadImage(const QString& type, const QString& name, const QString& url)
{
    if (url.isEmpty() || type == "error") {
//         qDebug() << "Webservice has no image for " << name;
        return;
    }
    // qDebug() << "Downloading image for " << name << " from " << url;
    QNetworkReply *reply = m_imageDownloadManager->get(QNetworkRequest(QUrl(url)));
    m_currentImageDownloads.insert(reply, QPair<QString,QString>(type, name));
}

void LastFmImageFetcher::gotImage(QNetworkReply* reply)
{
    const QPair<QString,QString> thisDownload = m_currentImageDownloads.take(reply);
    const QString type = thisDownload.first;
    const QString namePrefix = type + ':';
    const QString name = thisDownload.second;
    const QByteArray data = reply->readAll();

    QImage image = QImage::fromData(data);
    // qDebug() << "Adding image " << image.size() << " for " << name;
    SingletonFactory::instanceFor<PmcImageCache>()->addImage(QString(name).prepend(namePrefix), image);

    m_busy = false;
    QTimer::singleShot(0, this, SLOT(processQueue()));
    emit SingletonFactory::instanceFor<MediaLibrary>()->imageFetched(m_identifiers.take(name), name);
}

void LastFmImageFetcher::handleLastFmNewMedia(const QList< QSharedPointer<PmcMedia> > newMediaList)
{
    foreach(QSharedPointer<PmcMedia> media, newMediaList) {
        QString album = media->album();
        QString artist = media->artist();
        if (!album.isEmpty() && !artist.isEmpty()) {
            fetchImage("album", album, artist, album);
            fetchImage("artist", artist, artist);
        }
    }
}

void LastFmImageFetcher::run()
{
    QTimer::singleShot(0, this, &LastFmImageFetcher::setupImageFetcher);
    exec();
}

void LastFmImageFetcher::setupImageFetcher()
{
    m_netAccessManager = new QNetworkAccessManager;
    m_imageDownloadManager = new QNetworkAccessManager;
    connect(m_netAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotResponse(QNetworkReply*)));
    connect(m_imageDownloadManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotImage(QNetworkReply*)));
}

#include "lastfmimagefetcher.moc"
