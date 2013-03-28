
#include "rssbackend.h"

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

#include <krss/feedcollection.h>
#include <KRss/Item>

#include <KJob>
#include <KRandom>

using namespace Akonadi;

MEDIACENTER_EXPORT_BROWSINGBACKEND(RssBackend)

RssBackend::RssBackend (QObject* parent, const QVariantList& args):
	AbstractBrowsingBackend ( parent, args ),
	m_model(0),
	m_session(0),
	m_rootrecorder(0),
	m_feedrecorder(0),
	m_agentmanager(0),
	m_feeditemmodel(0)
{
}

QString RssBackend::backendCategory() const
{
    return "video";
}

bool RssBackend::expand ( int row )
{
	if(m_feeditemmodel) {
		return m_feeditemmodel->expand(row);
	}
	return MediaCenter::AbstractBrowsingBackend::expand(row);
}

bool RssBackend::goOneLevelUp()
{
	if(m_feeditemmodel) {
		return m_feeditemmodel->goOneLevelUp();
	}
	return MediaCenter::AbstractBrowsingBackend::goOneLevelUp();
}

bool RssBackend::initImpl()
{
	KRss::FeedCollection::registerAttributes();
	m_session = new Session( "mySession" );

	m_agentmanager = Akonadi::AgentManager::self();

	if (checkAgentInstance().isEmpty()) {
		requestNewAgent();
	} else {
        createEntityModel();
	}

    return true;
}

void RssBackend::collectionTreeFetched ( const Akonadi::Collection::List& collections )
{
	// search for rss collection
	Q_FOREACH(const KRss::FeedCollection &coll, collections) {
		if(!m_collection.isValid()) {
			if(coll.isValid() && coll.isFolder()) {
				m_collection = coll;
			}
		}
	}

	Akonadi::CollectionFetchScope cscope;
	cscope.setIncludeStatistics( true );
	cscope.setContentMimeTypes( QStringList() << KRss::Item::mimeType() );
	m_feedrecorder = new Akonadi::ChangeRecorder( this );
	m_feedrecorder->setSession( m_session );
	m_feedrecorder->fetchCollection( true );
	m_feedrecorder->setAllMonitored();
	m_feedrecorder->setCollectionFetchScope( cscope );
	m_feedrecorder->fetchCollectionStatistics( true );
	m_feedrecorder->setCollectionMonitored( m_collection );
	m_feedrecorder->itemFetchScope().fetchFullPayload();
	m_feedrecorder->itemFetchScope().fetchAllAttributes();
	m_feeditemmodel = new ProxyModel( m_feedrecorder, m_session, this );
	connect( m_feeditemmodel, SIGNAL(collectionPopulated()), this, SLOT(collectionPopulated()));
}

void RssBackend::collectionPopulated ()
{
    if (m_feeditemmodel->rowCount() == 0) {
        populateRessource();
    } else {
        setModel(m_feeditemmodel);
    }
}


void RssBackend::creationDone( KJob* job )
{
    if ( job->error() ) {
        qDebug() << i18n("Could not add feed: %1", job->errorString());
    } else {
        setModel(m_feeditemmodel);
    }
}

void RssBackend::createEntityModel()
{
    m_rootrecorder = new ChangeRecorder( this );
    m_rootrecorder->setCollectionMonitored( Collection::root() );
    m_rootrecorder->setMimeTypeMonitored( KRss::Item::mimeType() );
    m_rootrecorder->setSession( m_session );
    m_model = new EntityTreeModel( m_rootrecorder, this );
    m_model->setItemPopulationStrategy( EntityTreeModel::NoItemPopulation );
    connect( m_model, SIGNAL(collectionTreeFetched(Akonadi::Collection::List)), this, SLOT(collectionTreeFetched(Akonadi::Collection::List)));
}


QString RssBackend::checkAgentInstance()
{
	Akonadi::AgentInstance::List instances = m_agentmanager->instances();
	foreach ( const Akonadi::AgentInstance& instance, instances ) {
		if (instance.type().capabilities().contains("RssResource") && instance.type().mimeTypes().contains(KRss::Item::mimeType())) {
			return instance.type().identifier();
		}
	}
	return QString();
}

bool RssBackend::requestNewAgent()
{
	Akonadi::AgentType type = m_agentmanager->type("akonadi_krsslocal_resource");
	Akonadi::AgentInstanceCreateJob *job = new Akonadi::AgentInstanceCreateJob(type);
	connect( job, SIGNAL( result( KJob * ) ),
			 this, SLOT( agentCreated( KJob * ) ) );
	job->start();
	return true;
}

void RssBackend::agentCreated ( KJob* job )
{
	if ( job->error() == KJob::NoError ) {
		Akonadi::AgentInstanceCreateJob* createJob = qobject_cast<Akonadi::AgentInstanceCreateJob*>( job );
		Q_ASSERT( createJob );
		Akonadi::AgentInstance instance = createJob->instance();
        instance.setName(i18n("Plasma Media Center Rss Feeds"));
    	instance.synchronizeCollectionTree();
        // TODO save agent id

        createEntityModel();
	}
}

void RssBackend::populateRessource()
{
    // TODO feed configuration would be nice
    QString url = "http://feeds2.feedburner.com/AllJupiterVideos?format=xml";
    CachePolicy policy;
    policy.setInheritFromParent( false );
    policy.setSyncOnDemand( false );
    policy.setLocalParts( QStringList() << KRss::Item::HeadersPart << KRss::Item::ContentPart << Item::FullPayload );

    KRss::FeedCollection feed;
    feed.setRemoteId( url );
    feed.setXmlUrl( url );
    feed.setContentMimeTypes( QStringList( KRss::Item::mimeType() ) );
    feed.setCachePolicy( policy );
    feed.attribute<EntityDisplayAttribute>( Collection::AddIfMissing )->setIconName( KRss::Item::mimeType() );
    feed.setParentCollection( m_collection );
    feed.setName( url + KRandom::randomString( 8 ) );
    feed.setTitle( url );
    CollectionCreateJob* job = new CollectionCreateJob( feed );
    connect( job, SIGNAL(finished(KJob*)), this, SLOT(creationDone(KJob*)) );
    job->start();
}


