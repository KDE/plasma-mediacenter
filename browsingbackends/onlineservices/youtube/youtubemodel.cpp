/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "youtubemodel.h"
#include <libs/mediacenter/mediacenter.h>
// Qt
#include <QDomDocument>
#include <QDomNodeList>
// KDE
#include <kio/job.h>
#include <KUrl>
#include <QDebug>


YoutubeModel::YoutubeModel(QObject* parent): QAbstractListModel(parent)
{
    query("kde");
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

YoutubeModel::~YoutubeModel()
{

}

QVariant YoutubeModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case MediaCenter::HideLabelRole:
            return false;
        case Qt::DecorationRole:
            return m_videos.at(index.row()).thumbnail;
        case MediaCenter::MediaUrlRole:
            return m_videos.at(index.row()).url;
        case Qt::DisplayRole:
            return m_videos.at(index.row()).title;
        case MediaCenter::MediaTypeRole:
            return "video";
        case MediaCenter::IsExpandableRole:
            return true;
    }
    return QVariant();
}

int YoutubeModel::rowCount(const QModelIndex& parent) const
{
    return m_videos.count();
}

void YoutubeModel::query(const QString &searchTerm)
{
    if (searchTerm.isEmpty()) {
        return;
    }

    m_videos.clear();
    QString searchString = searchTerm;
    searchString.replace(' ', '+');
    // TODO: maybe reduce maximum results number allowed
    const QString url = "http://gdata.youtube.com/feeds/api/videos?q="
               + searchString
               + "&v=2";
    KUrl query(url);

    KIO::TransferJob *job = KIO::get(query, KIO::NoReload, KIO::HideProgressInfo);
    m_queries[job] = searchTerm;
    connect (job, SIGNAL(data(KIO::Job*, const QByteArray &)), this, SLOT(ytDataReady(KIO::Job*, const QByteArray &)));
    connect (job, SIGNAL(result(KJob *)), this, SLOT(parseResults(KJob*)));
}

void YoutubeModel::ytDataReady(KIO::Job *job, const QByteArray &data)
{
    // could this ever happen?
    if (!m_queries.contains(job)) {
        return;
    }

    m_datas[job].append(data);
}

void YoutubeModel::parseResults(KJob *job)
{
    if (!m_datas.contains(static_cast<KIO::Job*>(job))) {
        return;
    }

    QDomDocument document;
    document.setContent(m_datas[static_cast<KIO::Job*>(job)]);

    QDomNodeList entries = document.elementsByTagName("entry");
    for (int i = 0; i < entries.count(); i++) {
        QString id = entries.at(i).namedItem("id").toElement().text().split(":").last();
        QString title = entries.at(i).namedItem("title").toElement().text();
        QDomNode mediaNode = entries.at(i).namedItem("media:group");
        QString description = mediaNode.namedItem("media:description").toElement().text();
        QString keywords = mediaNode.namedItem("media:keywords").toElement().text();
        QString mediaUrl = mediaNode.namedItem("media:player").toElement().attribute("url");
        // FIXME: more than one media:thumbnail exists
        QString thumbnail = mediaNode.namedItem("media:thumbnail").toElement().attribute("url");

        QDomNode n = mediaNode.firstChild();
        QString MEDIA_CONTENT_URL;
        QString MEDIA_CONTENT_TYPE;
        do {
            if (n.nodeName() == "media:content" && n.toElement().attribute("yt:format") == "5") {
                MEDIA_CONTENT_URL = n.toElement().attribute("url");
                MEDIA_CONTENT_TYPE = n.toElement().attribute("type");
                break;
            }
            n = n.nextSibling();
        } while (n != mediaNode.lastChild());

        QString embeddedHTML = QString(
        "<object width=\"425\" height=\"350\">\n"
        "<param name=\"movie\" value=\"%1\"></param>\n"
        "<embed src=\"%2\"\n"
        "type=\"%3\" width=\"425\" height=\"350\">\n"
        "</embed>\n"
        "</object>\n").arg(MEDIA_CONTENT_URL, MEDIA_CONTENT_URL, MEDIA_CONTENT_TYPE);

        VideoPackage video;
        video.title = title;
        video.description = description;
        video.keywords = keywords.split(", ");
        video.id = id;
        video.duration = 0;
        video.embeddedHTML = embeddedHTML;
        video.thumbnail = thumbnail;
        video.url = mediaUrl;

        m_videos << video;
        reset();
    }
}
