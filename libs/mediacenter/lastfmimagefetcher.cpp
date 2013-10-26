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
#include "pmcimagecache.h"

#include <KGlobal>
#include <KDebug>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDomDocument>

#include <QTimer>
#include <QImage>

class LastFmImageFetcher::Singleton
{
public:
    LastFmImageFetcher q;
};

K_GLOBAL_STATIC( LastFmImageFetcher::Singleton, singleton )

LastFmImageFetcher *LastFmImageFetcher::instance()
{
    return &( singleton->q );
}

LastFmImageFetcher::LastFmImageFetcher(QObject* parent)
    : QObject(parent)
    , m_busy(false)
    , m_artistInfoUrl("http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&artist=%1&api_key=22a6906e49bffd8cc11be1385aea73de")
    , m_albumInfoUrl("http://ws.audioscrobbler.com/2.0/?method=album.getinfo&artist=%1&album=%2&api_key=22a6906e49bffd8cc11be1385aea73de")

{
    connect(&m_netAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotResponse(QNetworkReply*)));
    connect(&m_imageDownloadManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotImage(QNetworkReply*)));
}

LastFmImageFetcher::~LastFmImageFetcher()
{

}

void LastFmImageFetcher::fetchImage(const QString& type, const QPersistentModelIndex& index,
                                    const QString& artistName, const QString &albumName)
{
    QStringList nameList;
    nameList << type << artistName;
    if (!albumName.isEmpty()) {
        nameList << albumName;
    }
    m_pendingQueue.enqueue(nameList);
    m_modelIndexes.insert(albumName.isEmpty() ? artistName : albumName, index);
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

    kDebug() << "Fetching " << apiUrl;
    QNetworkReply *reply = m_netAccessManager.get(QNetworkRequest(apiUrl));
    m_currentInfoDownloads.insert(reply,
                                  nameList.count() > 2 ? nameList.at(2) : nameList.at(1));

    m_busy = true;
}

void LastFmImageFetcher::gotResponse(QNetworkReply* reply)
{
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

    kDebug() << "Webservice has no image for " << name;
    QTimer::singleShot(0, this, SLOT(processQueue()));
}

void LastFmImageFetcher::downloadImage(const QString& type, const QString& name, const QString& url)
{
    if (url.isEmpty() || type == "error") {
        kDebug() << "Webservice has no image for " << name;
        return;
    }
    kDebug() << "Downloading image for " << name << " from " << url;
    QNetworkReply *reply = m_imageDownloadManager.get(QNetworkRequest(url));
    m_currentImageDownloads.insert(reply, QPair<QString,QString>(type, name));
}

void LastFmImageFetcher::gotImage(QNetworkReply* reply)
{
    const QPair<QString,QString> thisDownload = m_currentImageDownloads.take(reply);
    const QString typePrefix = thisDownload.first + ':';
    const QString name = thisDownload.second;
    const QByteArray data = reply->readAll();

    QImage image = QImage::fromData(data);
    kDebug() << "Adding image " << image.size() << " for " << name;
    PmcImageCache::instance()->addImage(QString(name).prepend(typePrefix), image);

    m_busy = false;
    QTimer::singleShot(0, this, SLOT(processQueue()));

    emit imageFetched(m_modelIndexes.take(name), name);
}
