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

#ifndef AKONADIMODEL_H
#define AKONADIMODEL_H

#include <QtCore/QObject>


namespace Akonadi {
	class Session;
	class ChangeRecorder;
	class Collection;
}

namespace KRss {
	class FeedCollection;
}

class FeedController;
class ProxyModel;
class AgentManager;
class KJob;
class GpodderClient;
class QUrl;

class RssManager : public QObject
{
	Q_OBJECT
public:
    RssManager ( QObject* parent = 0, QString name = QString() );
public:
	void addFeed( const QString& feedurl );
	void deleteFeed( const QString& feedurl );
	void modifyFeed( const QString& feedurl );
	void addToplist();
signals:
	void modelPopulated( ProxyModel* model );
	void feedOperation(bool result);
private slots:
    void createRootResult( KJob* job );
    void createCollectionResult( KJob* job );
	void createModel( const Akonadi::Collection& coll );
	void agentRdy( const QString& id );
	void collectionFetchResult( KJob* job );
    void loadToplistItems( KJob* job );
private:
	Akonadi::Session *m_session;
	Akonadi::ChangeRecorder* m_feedrecorder;
	FeedController *m_feedcontroller;
	AgentManager *m_agentmanager;
	ProxyModel *m_feeditemmodel;
	QString m_rootcollname;
	GpodderClient *m_gpoclient;
};

#endif // AKONADIMODEL_H
