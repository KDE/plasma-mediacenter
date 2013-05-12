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

#include "agentmanager.h"

#include <Akonadi/CachePolicy>
#include <Akonadi/Collection>
#include <Akonadi/CollectionCreateJob>
#include <Akonadi/EntityDisplayAttribute>
#include <Akonadi/ChangeRecorder>
#include <Akonadi/EntityTreeModel>
#include <Akonadi/Session>
#include <Akonadi/CollectionFetchScope>
#include <Akonadi/ItemFetchScope>
#include <Akonadi/AgentManager>
#include <Akonadi/AgentInstanceCreateJob>

#include <KRss/Item>

#include <klocalizedstring.h>

AgentManager::AgentManager ( QObject* parent ) :
	QObject ( parent ),
	m_agentmanager(0)
{
	setup();
}

void AgentManager::setup()
{
	m_agentmanager = Akonadi::AgentManager::self();
}

void AgentManager::requestAgent()
{
	Akonadi::AgentInstance::List instances = m_agentmanager->instances();
	foreach ( const Akonadi::AgentInstance& instance, instances ) {
		if (instance.type().capabilities().contains("RssResource") && instance.type().mimeTypes().contains(KRss::Item::mimeType())) {
			m_agentid = instance.identifier();
			emit agentRdy(m_agentid);
			return;
		}
	}
	// agent not found, create new one
	requestNewAgent();
}

bool AgentManager::requestNewAgent()
{
	Akonadi::AgentType type = m_agentmanager->type("akonadi_krsslocal_resource");
	Akonadi::AgentInstanceCreateJob *job = new Akonadi::AgentInstanceCreateJob(type);
	connect( job, SIGNAL( result( KJob * ) ),
			 this, SLOT( agentCreated( KJob * ) ) );
	job->start();
	return true;
}

void AgentManager::agentCreated ( KJob* job )
{
	if ( job->error() == KJob::NoError ) {
		Akonadi::AgentInstanceCreateJob* createJob = qobject_cast<Akonadi::AgentInstanceCreateJob*>( job );
		Q_ASSERT( createJob );
		Akonadi::AgentInstance instance = createJob->instance();
		instance.setName(i18n("Plasma Media Center Rss Feeds"));
		instance.synchronizeCollectionTree();
		m_agentid = instance.identifier();
	} else {
		m_agentid = QString();
	}
	emit agentRdy(m_agentid);
}
