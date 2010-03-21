/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "flickrinterface.h"

// QT
#include <QDomDocument>
#include <QDomNodeList>

// KDE
#include <kio/job.h>
#include <KUrl>
#include <KDebug>

static const char* s_flickr_rest = "http://www.flickr.com/services/rest/";
// Flickr API key for Flickr On Plasma
static const char* s_apikey = "df52d83b7fe03f1de079da2c300f3203";
//static const char* s_secret = "";


FlickrInterface::FlickrInterface(QObject *parent) : QObject(parent)
{
}

FlickrInterface::~FlickrInterface()
{
}

void FlickrInterface::query(const QString &searchTerm)
{
    if ( searchTerm.isEmpty() )
        return;

    KUrl query(s_flickr_rest);
    query.addQueryItem("method", "flickr.photos.search");
    query.addQueryItem("api_key", s_apikey);
    query.addQueryItem("text", searchTerm);

    KIO::TransferJob *job = KIO::get(query, KIO::NoReload, KIO::HideProgressInfo);

    m_queries[job] = searchTerm;
    connect ( job, SIGNAL (data(KIO::Job*, const QByteArray &)), this, SLOT(flickrDataReady(KIO::Job*, const QByteArray &)) );
    connect ( job, SIGNAL( result(KJob*)), this, SLOT (parseResults(KJob*)) );
}

void FlickrInterface::flickrDataReady(KIO::Job *job, const QByteArray &data)
{
    if ( !m_queries.contains(job) )
        return;

    m_datas[job].append(data);

//    qDebug() << QString(data);
}


void FlickrInterface::parseResults(KJob *job)
{
    if ( !m_datas.contains(static_cast<KIO::Job*>(job) ) )
        return;

    listPhotos(job);

}


void FlickrInterface::listPhotos(KJob *job)
{
    QList<PicturePackage> pictures;

    QDomDocument document;
    if (document.setContent(m_datas[static_cast<KIO::Job*>(job)]))
        qDebug() << "Set Content Success";
    else
        qDebug() << "Set Content Fail";



    QDomNodeList entries = document.elementsByTagName("photo");

    QString title;
    QString id;
    QString owner;
    QString secret;
    QString farmID;
    QString serverID;

    for (int i = 0; i < entries.count(); i++)
    {
        QDomNamedNodeMap attr = entries.at(i).attributes();

        title = attr.namedItem("title").toAttr().value();
        id = attr.namedItem("id").toAttr().value();
        owner = attr.namedItem("owner").toAttr().value();
        secret = attr.namedItem("secret").toAttr().value();
        farmID = attr.namedItem("farm").toAttr().value();
        serverID = attr.namedItem("server").toAttr().value();

        PicturePackage picture;

        picture.id = id;
        picture.title = title;
        picture.description = "";
        picture.keywords << "";
        picture.author = owner;
        //picture.published = NULL;

        //picture.author = owner;
        //picture. = secret;
        //picture["farmID"] = farmID;
        //picture["serverID"] = serverID;
        // the "s" at the end stands for "small": 75x75 px

        //TODO: let options decide, what type of link user wants
        //now it is the default size: 500x375

        picture.link = QString("http://farm%1.static.flickr.com/%2/%3_%4.jpg")
                       .arg(farmID)
                       .arg(serverID)
                       .arg(id)
                       .arg(secret);
        picture.width = 0;
        picture.height = 0;
        picture.size = 0;
        picture.albumId = "";

        picture.thumbnail = QString("http://farm%1.static.flickr.com/%2/%3_%4_t.jpg")
                            .arg(farmID)
                            .arg(serverID)
                            .arg(id)
                            .arg(secret);

        pictures << picture;
    }
    emit result(m_queries[static_cast<KIO::Job*>(job)], pictures);
    m_queries.remove( static_cast<KIO::Job*>(job) );
    m_datas.remove( static_cast<KIO::Job*>(job) );
}
