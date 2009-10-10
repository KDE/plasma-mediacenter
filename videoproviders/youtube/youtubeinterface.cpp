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
#include "youtubeinterface.h"

// Qt
#include <QDomDocument>
#include <QDomNodeList>

// KDE
#include <kio/job.h>
#include <KUrl>


YouTubeInterface::YouTubeInterface(QObject *parent) : QObject(parent)
{
}

YouTubeInterface::~YouTubeInterface()
{}

void YouTubeInterface::query(const QString &searchTerm)
{
    if (searchTerm.isEmpty()) {
        return;
    }

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

void YouTubeInterface::ytDataReady(KIO::Job *job, const QByteArray &data)
{
    // could this ever happen?
    if (!m_queries.contains(job)) {
        return;
    }

    m_datas[job].append(data);
}

void YouTubeInterface::parseResults(KJob *job)
{
    if (!m_datas.contains(static_cast<KIO::Job*>(job))) {
        return;
    }

    QList<VideoPackage> videos;

    QDomDocument document;
    document.setContent(m_datas[static_cast<KIO::Job*>(job)]);

    QDomNodeList entries = document.elementsByTagName("entry");
    for (int i = 0; i < entries.count(); i++) {
        QString id = entries.at(i).namedItem("id").toElement().text().split(":").last();
        QString title = entries.at(i).namedItem("title").toElement().text();
        QDomNode mediaNode = entries.at(i).namedItem("media:group");
        QString description = mediaNode.namedItem("media:description").toElement().text();
        QString keywords = mediaNode.namedItem("media:keywords").toElement().text();

        // The embedding html code for youtube is:
        //
        // <object width="425" height="350">
        // <param name="movie" value="MEDIA_CONTENT_URL"></param>
        // <embed src="MEDIA_CONTENT_URL"
        // type="MEDIA_CONTENT_TYPE" width="425" height="350">
        // </embed>
        // </object>
        //
        // where MEDIA_CONTENT_URL has to be replaced by the url attribute of the media:content node
        // where the attribute yt:format is 5

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

        videos << video;
    }

    emit result(m_queries[static_cast<KIO::Job*>(job)], videos);
    m_queries.remove(static_cast<KIO::Job*>(job));
    m_datas.remove(static_cast<KIO::Job*>(job));

}
