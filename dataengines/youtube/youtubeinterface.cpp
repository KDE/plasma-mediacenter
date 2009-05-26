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
#include <KDebug>

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

    Plasma::DataEngine::Data video;
    QDomDocument document;
    document.setContent(m_datas[static_cast<KIO::Job*>(job)]);

    QDomNodeList entries = document.elementsByTagName("entry");
    kDebug() << entries.count();
    for (int i = 0; i < entries.count(); i++) {
        QString id = entries.at(i).namedItem("id").toElement().text().split(":").last();
        QString title = entries.at(i).namedItem("title").toElement().text();
        QDomNode mediaNode = entries.at(i).namedItem("media:group");
        QString description = mediaNode.namedItem("media:description").toElement().text();
    }

}
