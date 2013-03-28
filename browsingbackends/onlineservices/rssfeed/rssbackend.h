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
