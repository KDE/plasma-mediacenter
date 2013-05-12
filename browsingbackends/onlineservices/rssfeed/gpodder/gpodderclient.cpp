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
#include "gpodderrequesthandler.h"

#include <Solid/Networking>

#include <QTimer>

#include <mygpo-qt/ApiRequest.h>

#include <kdebug.h>

using namespace mygpo;

GpodderClient::GpodderClient ( QObject* parent, FeedController* controller ) :
	QObject ( parent ),
	m_nam(0),
	m_apirequest(0),
	m_feedcontroller(controller)
{
	m_nam = new QNetworkAccessManager(this);
    m_apirequest = new ApiRequest(m_nam);
}

GpodderClient::~GpodderClient()
{
	m_nam->deleteLater();
}

void GpodderClient::topFeeds ( const int count, const Akonadi::Collection& parent )
{
    if( Solid::Networking::status() == Solid::Networking::Unconnected )
    {
        QTimer::singleShot( 10000, this, SLOT(topFeeds()) );
        return;
    }

	PodcastListPtr topfeeds = m_apirequest->toplist(count);
    GpodderRequestHandler *rhandler = new GpodderRequestHandler(this, topfeeds, m_feedcontroller, parent);
	connect(topfeeds.data(), SIGNAL(requestError(QNetworkReply::NetworkError)), rhandler, SLOT(requestError(QNetworkReply::NetworkError)));
    connect(topfeeds.data(), SIGNAL(parseError()), rhandler, SLOT(parseError()));
    connect(topfeeds.data(), SIGNAL(finished()), rhandler, SLOT(finished()));
}

void GpodderClient::searchFeed ( const QString& query )
{
    ApiRequest req(m_nam);
}