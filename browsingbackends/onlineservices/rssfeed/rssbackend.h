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

#ifndef RSSBACKEND_H
#define RSSBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>

#include <Akonadi/Collection>

#include "proxymodel.h"
#include "rssmodel.h"

namespace Akonadi {
	class EntityTreeModel;
	class Session;
	class ChangeRecorder;
	class AgentManager;
}

class KJob;


class RssBackend : public MediaCenter::AbstractBrowsingBackend
{
	Q_OBJECT
public:
	RssBackend(QObject *parent, const QVariantList &args);

	virtual QString backendCategory() const;
	virtual bool expand(int row);
	virtual bool initImpl();
private:
    void createEntityModel();
    QString checkAgentInstance();
    bool requestNewAgent();
    void populateRessource();
private Q_SLOTS:
	void collectionTreeFetched( const Akonadi::Collection::List & collections );
	void collectionPopulated();
	void agentCreated( KJob* job );
    void creationDone( KJob* job );
public Q_SLOTS:
	virtual bool goOneLevelUp();

private:
	Akonadi::EntityTreeModel *m_model;
	Akonadi::Session *m_session;
	Akonadi::ChangeRecorder* m_rootrecorder;
	Akonadi::ChangeRecorder* m_feedrecorder;
	Akonadi::Collection m_collection;
	Akonadi::AgentManager* m_agentmanager;
	ProxyModel *m_feeditemmodel;

};

#endif // RSSBACKEND_H
