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

#ifndef GPODDERREQUESTHANDLER_H
#define GPODDERREQUESTHANDLER_H

#include <QtCore/QObject>

#include <mygpo-qt/PodcastList.h>

#include <Akonadi/Collection>

namespace Akonadi {
    class Collection;
}

class FeedController;

class GpodderRequestHandler : public QObject
{
    Q_OBJECT
public:
    GpodderRequestHandler ( QObject* parent, mygpo::PodcastListPtr podcasts, FeedController* controller, const Akonadi::Collection& parentcollection );
    virtual ~GpodderRequestHandler();

public slots:
    void finished();
    void requestError( QNetworkReply::NetworkError error );
    void parseError();

private:
    mygpo::PodcastListPtr m_podcasts;
    FeedController *m_feedcontroller;
    Akonadi::Collection m_parent;
};

#endif // GPODDERREQUESTHANDLER_H
