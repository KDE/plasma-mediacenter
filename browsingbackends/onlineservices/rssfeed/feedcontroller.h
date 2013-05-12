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

#ifndef FEEDCONTROLLER_H
#define FEEDCONTROLLER_H

#include <QtCore/QObject>

namespace Akonadi {
	class Collection;
}

namespace KRss {
    class FeedCollection;
}

class KJob;

class FeedController : public QObject
{
	Q_OBJECT
public:
    FeedController ( QObject* parent = 0 );
public:
	void addFeed( const QString& feedurl, const Akonadi::Collection& parent );
    KRss::FeedCollection newFeed( const QString& feedurl, const Akonadi::Collection& parent );
	void deleteFeed( const QString& feedurl );
	void modifyFeed( const QString& feedurl );
	void addFolder( const QString& name, const Akonadi::Collection& parent );
    KRss::FeedCollection newFolder(const QString& name, const Akonadi::Collection& parent);
signals:
	void feedOperation( const Akonadi::Collection& coll );
private slots:
	void collectionCreated( KJob* job );
};

#endif // FEEDCONTROLLER_H
