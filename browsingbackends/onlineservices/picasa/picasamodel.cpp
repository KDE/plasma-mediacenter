/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
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

#include "picasamodel.h"

#include <libs/mediacenter/mediacenter.h>
// Qt
#include <QDomDocument>
#include <QDomNodeList>

// KDE
#include <kio/job.h>
#include <KUrl>
#include <KDebug>

PicasaModel::PicasaModel(QObject* parent, const QString& username, const QString& password): QAbstractListModel(parent)
{
    m_flag = false;
    m_expandable = false;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    getTokenAndQuery(username, password,"album");
}


PicasaModel::~PicasaModel()
{

}

int PicasaModel::rowCount(const QModelIndex& parent) const
{
    return (m_expandable ?  m_albums.count() : m_photos.count());
}


QVariant PicasaModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case MediaCenter::HideLabelRole:
            return false;
        case Qt::DecorationRole:
            return (m_expandable ? m_albums.at(index.row()).thumbnail : m_photos.at(index.row()).thumbnail144);
        case MediaCenter::MediaUrlRole:
            return (m_expandable ? m_albums.at(index.row()).link : m_photos.at(index.row()).link);
        case Qt::DisplayRole:
            return (m_expandable ? (m_albums.at(index.row()).title + " (" + m_albums.at(index.row()).noOfPhotos + "Photos)") : "");
        case MediaCenter::MediaTypeRole:
            return "image";
        case MediaCenter::IsExpandableRole:
            return m_expandable;
    }
    return QVariant();
}

void PicasaModel::query(const QString &username, const QString &request)
{
    if (username.isEmpty()) {
       m_flag = false;
        return;
    }

    // if we require photos from an album, we have to split request to
    // obtain the albumid; ex: photo/32323232432
    if (request.contains("/")) {;
        m_albumid = request.split('/').last();
        m_request = request.split('/').first();
    } else {
        m_request = request;
    }

    QString searchString = username;

    QString url = "http://picasaweb.google.com/data/feed/api/user/" + username;
    if (m_request.contains("photo")) {
        url.append("/albumid/" +  m_albumid);
    }
    //url.append("?kind=" + m_request);

    KUrl query(url);

    KIO::TransferJob *job = KIO::get(query, KIO::NoReload, KIO::HideProgressInfo);

    if (!m_token.isEmpty()) {
        m_flag = true;
        QString auth_string = "GoogleLogin auth=" + m_token;
        job->addMetaData("content-type", "Content-Type: application/x-www-form-urlencoded");
        job->addMetaData("customHTTPHeader", "Authorization: " + auth_string);
    }
    m_queries[job] = username;
    connect (job, SIGNAL(data(KIO::Job*,QByteArray)), this, SLOT(picasaDataReady(KIO::Job*,QByteArray)));
    connect (job, SIGNAL(result(KJob *)), this, SLOT(parseResults(KJob*)));
}

void PicasaModel::picasaDataReady(KIO::Job *job, const QByteArray &data)
{
    // could this ever happen?
    if (!m_queries.contains(job)) {
        return;
    }

    m_datas[job].append(data);
}

void PicasaModel::parseResults(KJob *job)
{
    if (!m_datas.contains(static_cast<KIO::Job*>(job))) {
        return;
    }

    m_token = "";

    if (m_request.contains("album")) {
        listAllAlbums(job);
    } else if (m_request.contains("photo")) {
        listAllPhotos(job);
    }
}

void PicasaModel::listAllAlbums(KJob *job)
{
    QDomDocument document;
    document.setContent(m_datas[static_cast<KIO::Job*>(job)]);

    QDomNodeList entries = document.elementsByTagName("entry");

    m_albums.clear();
    for (int i = 0; i < entries.count(); i++) {

        QString id = entries.at(i).namedItem("id").toElement().text().split('/').last();
        QString published = entries.at(i).namedItem("published").toElement().text();
        QString updated = entries.at(i).namedItem("updated").toElement().text();
        m_expandable = true;

        // link is the direct link to the album
        QDomElement domElement = entries.at(i).firstChildElement("link");
        while (!domElement.attribute("type").contains("text/html") && !domElement.isNull()) {
            //value = domElement.attribute("type");
            domElement = domElement.nextSiblingElement("link");
        }
        QString link = domElement.attribute("href");

        QString title = entries.at(i).namedItem("title").toElement().text();
        QString summary = entries.at(i).namedItem("summary").toElement().text();
        QString numPhotos = entries.at(i).namedItem("gphoto:numphotos").toElement().text();
        // byteUsed only if the user is logged
        QString byteUsed = entries.at(i).namedItem("gphoto:bytesUsed").toElement().text();
        QDomNode mediaNode = entries.at(i).namedItem("media:group");
        QDomElement mediaElement = mediaNode.firstChildElement("media:thumbnail");
        QString thumb = mediaElement.attribute("url");

        Album album;
        album.id = id;
        album.published = published;
        album.updated = updated;
        album.title = title;
        album.link = link;
        album.summary = summary;
        album.noOfPhotos = numPhotos;
        album.thumbnail = thumb;

        m_albums.append(album);

//         if (!byteUsed.isEmpty()) {
//             album["byte used"] = byteUsed;
//         }

    }
    m_queries.remove(static_cast<KIO::Job*>(job));
    m_datas.remove(static_cast<KIO::Job*>(job));
    reset();

}

void PicasaModel::listAllPhotos(KJob *job)
{
    QDomDocument document;
    document.setContent(m_datas[static_cast<KIO::Job*>(job)]);

    QDomNodeList entries = document.elementsByTagName("entry");
    m_expandable = false;
    m_photos.clear();
    for (int i = 0; i < entries.count(); i++) {

        QString id = entries.at(i).namedItem("id").toElement().text().split('/').last();
        QString published = entries.at(i).namedItem("published").toElement().text();
        QString updated = entries.at(i).namedItem("updated").toElement().text();
        QString title = entries.at(i).namedItem("title").toElement().text();
        QDomElement contentElement = entries.at(i).firstChildElement("content");
        QString link = contentElement.attribute("src");
        QString albumid = entries.at(i).namedItem("gphoto:albumid").toElement().text();
        QString width = entries.at(i).namedItem("gphoto:width").toElement().text();
        QString height = entries.at(i).namedItem("gphoto:height").toElement().text();
        QString size = entries.at(i).namedItem("gphoto:size").toElement().text();

        QDomNode mediaNode = entries.at(i).namedItem("media:group");
        QDomElement mediaElement = mediaNode.firstChildElement("media:thumbnail");
        QString thumb72 = mediaElement.attribute("url");
        mediaElement = mediaElement.nextSiblingElement("media:thumbnail");
        QString thumb144 = mediaElement.attribute("url");
        mediaElement = mediaElement.nextSiblingElement("media:thumbnail");
        QString thumb288 = mediaElement.attribute("url");

        Photo photo;
        photo.published = published;
        photo.updated = updated;
        photo.title = title;
        photo.link = link;
        photo.albumId = albumid;
        photo.width = width;
        photo.height = height;
        photo.size = size;

        photo.thumbnail72 = thumb72;
        photo.thumbnail144 = thumb144;
        photo.thumbnail288 = thumb288;

        m_photos.append(photo);
    }
    m_queries.remove(static_cast<KIO::Job*>(job));
    m_datas.remove(static_cast<KIO::Job*>(job));
    reset();

}


void PicasaModel::getTokenAndQuery(const QString &username, const QString &password, const QString &request)
{
    m_request = request;
    m_username = username;

    KUrl url("https://www.google.com/accounts/ClientLogin");
    QString accountType = "GOOGLE";
    QStringList qsl;
    qsl.append("Email="+username);
    qsl.append("Passwd="+password);
    qsl.append("accountType="+accountType);
    qsl.append("service=lh2");
    qsl.append("source=kde-picasaengine-0.1");
    QString dataParameters = qsl.join("&");
    QByteArray buffer;
    buffer.append(dataParameters.toUtf8());

    KIO::TransferJob *job = KIO::http_post(url, buffer, KIO::HideProgressInfo);
    job->addMetaData("content-type", "Content-Type: application/x-www-form-urlencoded" );
    connect (job, SIGNAL(data(KIO::Job*,QByteArray)), this, SLOT(token(KIO::Job*,QByteArray)));
    connect (job, SIGNAL(result(KJob *)), this, SLOT(passwordJob(KJob *)));

}

void PicasaModel::token(KIO::Job *job, const QByteArray &data)
{
    Q_UNUSED(job);

    if (data.isEmpty())
        return;

    QString output(data);

    if (output.contains("Auth=")) {
        emit loginComplete(true);
    } else {
        emit loginComplete(false);
    }

    if (output.contains("Auth=")) {
        QStringList strList = output.split("Auth=");
        if (strList.count() > 0) {
            m_token = strList[1].trimmed();
        }
    }
}

void PicasaModel::passwordJob(KJob *job)
{
    Q_UNUSED(job);

    if (m_token.isEmpty()) {
        return;
    }

    query(m_username, m_request);
}

bool PicasaModel::browseToAlbum(int row)
{
    query(m_username, ("photos/" + m_albums.at(row).id));
    return true;
}

bool PicasaModel::goBack()
{
    if(!m_expandable && m_flag) {
        query(m_username, "album");
        return true;
    } else {
        return false;
    }
}
