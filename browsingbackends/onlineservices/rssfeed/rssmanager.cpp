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

#include "rssmanager.h"

#include <Akonadi/Collection>
#include <Akonadi/ChangeRecorder>
#include <Akonadi/CachePolicy>
#include <Akonadi/EntityTreeModel>
#include <Akonadi/Session>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/CollectionCreateJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/EntityDisplayAttribute>

#include <QEventLoop>
#include <QTimer>

#include <KRss/FeedCollection>
#include <KRss/Item>

#include "proxymodel.h"
#include "feedcontroller.h"
#include "agentmanager.h"
#include "gpodder/gpodderclient.h"


RssManager::RssManager ( QObject* parent, QString name ) :
	QObject ( parent ),
	m_session(0),
	m_feedrecorder(0),
	m_feedcontroller(0),
	m_agentmanager(0),
	m_feeditemmodel(0),
	m_rootcollname(name),
	m_gpoclient(0)
{
	KRss::FeedCollection::registerAttributes();
	m_session = new Akonadi::Session( "PMC Rss Session" );
	m_agentmanager = new AgentManager(this);
	connect(m_agentmanager, SIGNAL(agentRdy(const QString&)), this, SLOT(agentRdy(const QString&)));
	m_agentmanager->requestAgent();
	m_feedcontroller = new FeedController(this);
	m_gpoclient = new GpodderClient(this, m_feedcontroller);
}

void RssManager::addFeed ( const QString& feedurl )
{
	if (m_feeditemmodel) {
        Akonadi::CollectionCreateJob* job = new Akonadi::CollectionCreateJob(
            m_feedcontroller->newFeed(feedurl, m_feeditemmodel->parent()) );

        connect( job, SIGNAL(result(KJob*)), SLOT(createCollectionResult(KJob*)) );
        job->start();
	}
}

void RssManager::deleteFeed ( const QString& feedurl )
{
	m_feedcontroller->deleteFeed(feedurl);
}

void RssManager::modifyFeed ( const QString& feedurl )
{
	m_feedcontroller->modifyFeed(feedurl);
}

void RssManager::addToplist()
{
    if (m_feeditemmodel) {
        Akonadi::CollectionCreateJob* job = new Akonadi::CollectionCreateJob(
            m_feedcontroller->newFolder(QString("Gpodder toplist"), m_feeditemmodel->parent()) );

        connect( job, SIGNAL(finished(KJob*)), SLOT(loadToplistItems(KJob*)) );
        job->start();
    }
}

void RssManager::createRootResult ( KJob* job )
{
    if (!job->error()) {
        Akonadi::CollectionCreateJob* createJob = qobject_cast<Akonadi::CollectionCreateJob*>(job);
        Q_ASSERT(createJob);
        createModel(createJob->collection());
    }
}

void RssManager::createCollectionResult ( KJob* job )
{
    if (job->error()) {
        kDebug() << job->errorText();
        emit feedOperation(false);
    } else {
        emit feedOperation(true);
    }
}

void RssManager::createModel ( const Akonadi::Collection& coll )
{
    kDebug() << "createModel" << coll;
	// create model
	Akonadi::CollectionFetchScope cscope;
	cscope.setIncludeStatistics( true );
	cscope.setContentMimeTypes( QStringList() << KRss::Item::mimeType() );
	m_feedrecorder = new Akonadi::ChangeRecorder( this );
	m_feedrecorder->setSession( m_session );
	m_feedrecorder->fetchCollection( true );
	m_feedrecorder->setAllMonitored();
	m_feedrecorder->setCollectionFetchScope( cscope );
	m_feedrecorder->fetchCollectionStatistics( true );
	m_feedrecorder->setCollectionMonitored( coll );
	m_feedrecorder->itemFetchScope().fetchFullPayload();
	m_feedrecorder->itemFetchScope().fetchAllAttributes();
	m_feeditemmodel = new ProxyModel( m_feedrecorder, this );

    emit modelPopulated(m_feeditemmodel);
}

void RssManager::agentRdy ( const QString& id )
{
	Akonadi::CollectionFetchJob *job = new Akonadi::CollectionFetchJob( Akonadi::Collection::root(),
																		Akonadi::CollectionFetchJob::Recursive,
																	 this );
	job->fetchScope().setResource(id);
	connect( job, SIGNAL( result( KJob* ) ), this, SLOT( collectionFetchResult( KJob* ) ) );
}

void RssManager::collectionFetchResult ( KJob* job )
{
	if ( job->error() == KJob::NoError ) {
		Akonadi::CollectionFetchJob* fetchJob = qobject_cast<Akonadi::CollectionFetchJob*>(job);
		Q_ASSERT(fetchJob);
		Akonadi::Collection::List collList = fetchJob->collections();
		// look for our parent collection
		Q_FOREACH( const Akonadi::Collection &coll, collList ) {
			if( coll.name() == m_rootcollname ) {
                kDebug() << "foundrootcoll";
				createModel(coll);
				return;
			}
		}

		// we need to find a top level collection
		Q_FOREACH(const KRss::FeedCollection &coll, collList) {
			if(coll.isValid() && coll.isFolder()) {
				if (coll.parentCollection() == Akonadi::Collection::root()) {
					const KRss::FeedCollection rootCool = KRss::FeedCollection(coll);
                    kDebug() << "add" << m_rootcollname;
                    Akonadi::CollectionCreateJob* job = new Akonadi::CollectionCreateJob( m_feedcontroller->newFolder(m_rootcollname, rootCool) );
                    connect( job, SIGNAL(finished(KJob*)), SLOT(createRootResult(KJob*)) );
                    job->start();
					return;
				}
			}
		}
	}
}

void RssManager::loadToplistItems ( KJob* job )
{
    if (!job->error()) {
        Akonadi::CollectionCreateJob* createJob = qobject_cast<Akonadi::CollectionCreateJob*>(job);
        Q_ASSERT(createJob);
		m_gpoclient->topFeeds(3, createJob->collection());
	}
}
