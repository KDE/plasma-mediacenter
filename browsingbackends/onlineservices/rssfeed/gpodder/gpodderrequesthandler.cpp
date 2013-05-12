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

#include "gpodderrequesthandler.h"
#include "../feedcontroller.h"

#include <mygpo-qt/PodcastList.h>
#include <mygpo-qt/Podcast.h>
#include <mygpo-qt/ApiRequest.h>

#include <kdebug.h>

using namespace mygpo;

GpodderRequestHandler::GpodderRequestHandler ( QObject* parent, mygpo::PodcastListPtr podcasts, FeedController* controller, const Akonadi::Collection& parentcollection ) :
    QObject ( parent ),
    m_podcasts( podcasts ),
    m_feedcontroller(controller),
    m_parent(parentcollection)
{

}

GpodderRequestHandler::~GpodderRequestHandler()
{

}

void GpodderRequestHandler::finished()
{
    QList<PodcastPtr> list;
    QList<PodcastPtr>::const_iterator iterator;
    list = m_podcasts.data()->list();
    for (iterator = list.constBegin(); iterator != list.constEnd(); ++iterator) {
        m_feedcontroller->addFeed(iterator->data()->url().toString(), m_parent);
    }
    deleteLater();
}

void GpodderRequestHandler::requestError ( QNetworkReply::NetworkError error )
{
    kDebug() << "requestError" << error;
    deleteLater();
}

void GpodderRequestHandler::parseError()
{
    kDebug() << "error in parsing gpodder podcasts";
    deleteLater();
}
