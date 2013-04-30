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

#include "feedcontroller.h"

#include <Akonadi/CachePolicy>
#include <Akonadi/CollectionCreateJob>
#include <Akonadi/CollectionFetchJob>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/EntityDisplayAttribute>

#include <KRss/FeedCollection>
#include <KRss/Item>

#include <KRandom>


FeedController::FeedController ( QObject* parent ) :
	QObject ( parent )
{
}

void FeedController::addFeed( const QString& feedurl, const Akonadi::Collection& parent )
{
	Akonadi::CachePolicy policy;
	policy.setInheritFromParent( false );
	policy.setSyncOnDemand( false );
	policy.setLocalParts( QStringList() << KRss::Item::HeadersPart << KRss::Item::ContentPart << Akonadi::Item::FullPayload );

	KRss::FeedCollection feed;
	feed.setRemoteId( feedurl );
	feed.setXmlUrl( feedurl );
	feed.setContentMimeTypes( QStringList( KRss::Item::mimeType() ) );
	feed.setCachePolicy( policy );
	feed.attribute<Akonadi::EntityDisplayAttribute>( Akonadi::Collection::AddIfMissing )->setIconName( KRss::Item::mimeType() );
	feed.setParentCollection( parent );
	feed.setName( feedurl + KRandom::randomString( 8 ) );
	feed.setTitle( feedurl );
	Akonadi::CollectionCreateJob* job = new Akonadi::CollectionCreateJob( feed );
	connect( job, SIGNAL(finished(KJob*)), this, SLOT(collectionCreated(KJob*)) );
	job->start();
}

void FeedController::deleteFeed ( const QString& feedurl )
{
	return;
}

void FeedController::modifyFeed ( const QString& feedurl )
{
	return;
}

void FeedController::addCollection ( const QString& name, const Akonadi::Collection& parent )
{
	KRss::FeedCollection folder;
	folder.setParentCollection( parent );
	folder.setRemoteId( name );
	folder.setIsFolder( true );
	folder.setName( name );
	folder.setTitle( name );
	folder.setContentMimeTypes( QStringList() << Akonadi::Collection::mimeType() << KRss::Item::mimeType() );
	folder.attribute<Akonadi::EntityDisplayAttribute>( Akonadi::Collection::AddIfMissing )->setDisplayName( name );
	folder.setRights( Akonadi::Collection::CanCreateCollection );

	Akonadi::CollectionCreateJob* job = new Akonadi::CollectionCreateJob( folder );
	connect( job, SIGNAL(finished(KJob*)), this, SLOT(collectionCreated(KJob*)) );
	job->start();
}

void FeedController::collectionCreated ( KJob* job )
{
	kDebug() << job->errorString();
	if ( job->error() == KJob::NoError ) {
		Akonadi::CollectionCreateJob* createJob = qobject_cast<Akonadi::CollectionCreateJob*>(job);
		Q_ASSERT(createJob);
// 		KRss::FeedCollection* fcoll = new KRss::FeedCollection(createJob->collection());
		emit feedOperation(createJob->collection());
		return;
	}
	emit feedOperation(Akonadi::Collection());
}