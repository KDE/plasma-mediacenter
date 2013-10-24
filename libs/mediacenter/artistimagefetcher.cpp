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

#include "artistimagefetcher.h"
#include "pmcimagecache.h"

#include <KDE/KGlobal>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDomDocument>

#include <QDebug>
#include <QTimer>
#include <QImage>

class ArtistImageFetcher::Singleton
{
public:
    ArtistImageFetcher q;
};

K_GLOBAL_STATIC( ArtistImageFetcher::Singleton, singleton )

ArtistImageFetcher *ArtistImageFetcher::instance()
{
    return &( singleton->q );
}

const char *ArtistImageFetcher::artistIdentification = "artist:";

ArtistImageFetcher::ArtistImageFetcher(QObject* parent)
    : QObject(parent)
    , m_busy(false)
    , m_artistInfoUrl("http://ws.audioscrobbler.com/2.0/?method=artist.getinfo&artist=%1&api_key=22a6906e49bffd8cc11be1385aea73de")

{
    connect(&m_netAccessManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotResponse(QNetworkReply*)));
    connect(&m_imageDownloadManager, SIGNAL(finished(QNetworkReply*)), SLOT(gotImage(QNetworkReply*)));
}

ArtistImageFetcher::~ArtistImageFetcher()
{

}

void ArtistImageFetcher::fetchArtistImage(const QString& artistImage)
{
    m_artistQueue.enqueue(artistImage);
    QTimer::singleShot(0, this, SLOT(processQueue()));
}

void ArtistImageFetcher::processQueue()
{
    if (m_artistQueue.empty()) {
        QTimer::singleShot(1000, this, SLOT(processQueue()));
        return;
    }

    const QString nextArtist = m_artistQueue.dequeue();
    m_netAccessManager.get(QNetworkRequest(QUrl(m_artistInfoUrl.arg(nextArtist))));
    m_busy = true;
}

void ArtistImageFetcher::gotResponse(QNetworkReply* reply)
{
    QDomDocument doc;
    doc.setContent(reply->readAll());

    const QDomElement artistElement = doc.firstChildElement().firstChildElement();

    const QString artistName = artistElement.elementsByTagName("name").at(0).toElement().text();
    const QDomNodeList imageList = artistElement.childNodes();

    for (int i=imageList.length(); i>0; i--) {
        const QDomElement element = imageList.at(i).toElement();
        if (element.tagName() == "image" &&
            (element.attribute("size") == "extralarge"
                || element.attribute("size") == "large")) {
            downloadImage(artistName, element.text());
            return;
        }
    }
}

void ArtistImageFetcher::downloadImage(const QString& artistName, const QString& url)
{
    if (url.isEmpty())
        return;
    qDebug() << "Downloading image for " << artistName << " from " << url;
    QNetworkReply *reply = m_imageDownloadManager.get(QNetworkRequest(url));
    m_currentArtistImageDownloads.insert(reply, artistName);
}

void ArtistImageFetcher::gotImage(QNetworkReply* reply)
{
    QString artistName = m_currentArtistImageDownloads.take(reply);
    const QByteArray data = reply->readAll();

    QImage image = QImage::fromData(data);
    qDebug() << "Adding image " << image.size() << " for " << artistName;
    PmcImageCache::instance()->addImage(artistName.prepend(artistIdentification), image);

    m_busy = false;
    QTimer::singleShot(0, this, SLOT(processQueue()));
}
