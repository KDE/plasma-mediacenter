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

#ifndef GPODDERCLIENT_H
#define GPODDERCLIENT_H

#include <QtCore/QObject>
#include <QNetworkReply>

#include <mygpo-qt/PodcastList.h>


class QNetworkAccessManager;
class FeedController;

namespace mygpo {
    class ApiRequest;
}

namespace Akonadi {
    class Collection;
}

class GpodderClient : public QObject
{
	Q_OBJECT
public:
    GpodderClient ( QObject* parent, FeedController* controller );
    ~GpodderClient();
public slots:
    void topFeeds( const int count, const Akonadi::Collection& parent );
    void searchFeed(const QString &query);
private:
	QNetworkAccessManager *m_nam;
    mygpo::ApiRequest *m_apirequest;
    FeedController *m_feedcontroller;
};

#endif // GPODDERCLIENT_H
