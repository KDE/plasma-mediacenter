/*****
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

#include "flickrmodel.h"

#include <libs/mediacenter/mediacenter.h>

#include <KDE/KUrl>
#include <KDE/KIO/Job>
#include <KDE/KJob>

#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>

#include <QDebug>

namespace {
    const char* s_flickr_rest = "http://www.flickr.com/services/rest/";
    // Flickr API key for Flickr On Plasma
    const char* s_apikey = "df52d83b7fe03f1de079da2c300f3203";
    //static const char* s_secret = "";
}

FlickrModel::FlickrModel(QObject* parent): QAbstractListModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    query("kde");
}

FlickrModel::~FlickrModel()
{

}

void FlickrModel::query (const QString& searchTerm)
{
    if ( searchTerm.isEmpty() )
    return;

    KUrl query(s_flickr_rest);
    query.addQueryItem("method", "flickr.photos.search");
    query.addQueryItem("api_key", s_apikey);
    query.addQueryItem("text", searchTerm);
    query.addQueryItem("extras", "url_o,url_b");

    KIO::TransferJob *job = KIO::get(query, KIO::NoReload, KIO::HideProgressInfo);

    m_queries[job] = searchTerm;
    connect ( job, SIGNAL(data(KIO::Job*,QByteArray)), this,
              SLOT(flickrDataReady(KIO::Job*,QByteArray)) );
    connect ( job, SIGNAL(result(KJob*)), this, SLOT (parseResults(KJob*)) );
}

void FlickrModel::flickrDataReady (KIO::Job* job, const QByteArray& data)
{
    if ( !m_queries.contains(job) )
        return;

    m_datas[job].append(data);
}

void FlickrModel::listPhotos (KJob* job)
{
    QDomDocument document;
    m_photos.clear();
    if (document.setContent(m_datas[static_cast<KIO::Job*>(job)]))
        qDebug() << "Set Content Success";
    else
        qDebug() << "Set Content Fail";

    QDomNodeList entries = document.elementsByTagName("photo");

    for (int i = 0; i < entries.count(); i++)
    {
        QDomNamedNodeMap attr = entries.at(i).attributes();
        const QString title = attr.namedItem("title").toAttr().value();
        const QString id = attr.namedItem("id").toAttr().value();
        const QString owner = attr.namedItem("owner").toAttr().value();
        const QString secret = attr.namedItem("secret").toAttr().value();
        const QString farmID = attr.namedItem("farm").toAttr().value();
        const QString serverID = attr.namedItem("server").toAttr().value();
        const QString bigImageUrl = attr.namedItem("url_b").toAttr().value();;
        const QString originalImageUrl = attr.namedItem("url_o").toAttr().value();;

        Photo photo;
        photo.title = title;
        photo.id = id;
        photo.owner= owner;
        photo.secret = secret;
        photo.secret = farmID;
        photo.secret = serverID;
        // link of image fetched
        photo.previewImgLink = QString("http://farm%1.static.flickr.com/%2/%3_%4_n.jpg")
                        .arg(farmID)
                        .arg(serverID)
                        .arg(id)
                        .arg(secret);
        photo.originalImgLink = originalImageUrl.isEmpty() ? (bigImageUrl.isEmpty() ? photo.previewImgLink : bigImageUrl)
                                                           : originalImageUrl;
        m_photos.append(photo);
    }

    m_queries.remove( static_cast<KIO::Job*>(job) );
    m_datas.remove( static_cast<KIO::Job*>(job) );

    reset();
}

void FlickrModel::parseResults (KJob* job)
{
    if ( !m_datas.contains(static_cast<KIO::Job*>(job) ) )
        return;

    listPhotos(job);
}

QVariant FlickrModel::data (const QModelIndex& index, int role) const
{
    switch (role) {
    case MediaCenter::HideLabelRole:
        return true;
    case Qt::DecorationRole:
        return m_photos.at(index.row()).previewImgLink;
    case MediaCenter::MediaUrlRole:
        return m_photos.at(index.row()).originalImgLink;
    case MediaCenter::MediaTypeRole:
        return "image";
    }

    return QVariant();
}

int FlickrModel::rowCount (const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_photos.count();
}

ModelMetadata* FlickrModel::metadata()
{
    return &m_metadata;
}
