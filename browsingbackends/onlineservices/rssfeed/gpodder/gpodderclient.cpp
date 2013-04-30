/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gpodderclient.h"

#include <mygpo-qt/ApiRequest.h>

#include <kdebug.h>

using namespace mygpo;

GpodderClient::GpodderClient ( QObject* parent ) :
	QObject ( parent ),
	m_nam(0),
	m_podcastlist(0)
{
	m_nam = new QNetworkAccessManager(this);
}

GpodderClient::~GpodderClient()
{
	m_nam->deleteLater();
}

void GpodderClient::topFeeds ( const int count )
{
	ApiRequest req(m_nam);
	m_podcastlist = new PodcastListPtr(req.toplist(count));
	connect(m_podcastlist->data(), SIGNAL(requestError(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
	connect(m_podcastlist->data(), SIGNAL(finished()), this, SLOT(requestFinished()));
}

void GpodderClient::networkError ( QNetworkReply::NetworkError err )
{
	//TODO
	kDebug() << "network err" << err;
}

void GpodderClient::requestFinished()
{
	QList<QUrl> urllist;
	QList<PodcastPtr> list;
	QList<PodcastPtr>::const_iterator iterator;

	list = m_podcastlist->data()->list();
	for (iterator = list.constBegin(); iterator != list.constEnd(); ++iterator) {
		urllist.append(iterator->data()->url());
	}

	emit podcastList(&urllist);
}
