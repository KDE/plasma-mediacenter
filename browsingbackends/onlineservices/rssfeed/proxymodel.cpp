
#include "proxymodel.h"

#include <libs/mediacenter/mediacenter.h>
#include "rssmodel.h"

#include <Akonadi/CachePolicy>
#include <Akonadi/Collection>
#include <Akonadi/ChangeRecorder>
#include <Akonadi/EntityTreeModel>
#include <Akonadi/Session>

#include <KRss/FeedCollection>


class ProxyModel::Private
{
public:
	RssModel* m_feedmodel;
	RssModel* m_treemodel;
	KDescendantsProxyModel* m_flatmodel;
	Akonadi::ChangeRecorder* m_recorder;
	Akonadi::Session* m_session;
	Akonadi::Collection* m_parentcollection;
	QRegExp m_parentcollectionfilter;
	QRegExp m_collectionfilter;
};

ProxyModel::ProxyModel ( Akonadi::ChangeRecorder* monitor, Akonadi::Session* session, QObject* parent ) :
	QSortFilterProxyModel ( parent ),
	d( new Private() )
{
	setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
	KRss::FeedCollection::registerAttributes();
	d->m_recorder = monitor;
	d->m_session = session;

	d->m_parentcollection = new Akonadi::Collection(monitor->collectionsMonitored().first());

	d->m_collectionfilter = QRegExp(d->m_parentcollection->remoteId(), Qt::CaseSensitive,
									QRegExp::FixedString);
	d->m_treemodel = new RssModel(d->m_recorder, this);

	setFilterRole(Akonadi::EntityTreeModel::RemoteIdRole);
	setFilterRegExp(d->m_collectionfilter);
	connect(d->m_treemodel, SIGNAL(collectionPopulated(Akonadi::Collection::Id)), this, SLOT(treemodelrdy()));
}

ProxyModel::~ProxyModel () {
	delete d;
}

bool ProxyModel::expand ( int row )
{
	KDescendantsProxyModel* m = qobject_cast<KDescendantsProxyModel*>(sourceModel());
	Akonadi::Collection selectedColl = m->index(row, 0).data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
	QString remoteId = m->index(row, 0).data(Akonadi::EntityTreeModel::RemoteIdRole).toString();
	kDebug() << "remoteId" << m->index(row, 0).data(Akonadi::EntityTreeModel::RemoteIdRole).toString();
	d->m_parentcollectionfilter = d->m_collectionfilter;
	d->m_collectionfilter.setPattern(remoteId);
	setFilterRegExp(d->m_collectionfilter);
	return true;
}

bool ProxyModel::goOneLevelUp()
{
	if(d->m_parentcollection) {
		const QString parent = d->m_parentcollection->remoteId();
		d->m_collectionfilter.setPattern(parent);
		setFilterRegExp(d->m_collectionfilter);
		return true;
	}
	return false;
}

bool ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	const QModelIndex i = sourceModel()->index(sourceRow, 0, sourceParent);
	const Akonadi::Collection parent = i.data(Akonadi::EntityTreeModel::ParentCollectionRole).value<Akonadi::Collection>();
	if(!filterRegExp().pattern().isEmpty()) {
		return filterRegExp().exactMatch(parent.remoteId());
    }

	return QSortFilterProxyModel::filterAcceptsRow( sourceRow, sourceParent );
}

void ProxyModel::collPopulated(Akonadi::Collection::Id cid)
{
	setSourceModel(d->m_feedmodel);
	emit collectionPopulated();
}

void ProxyModel::treemodelrdy()
{
	d->m_flatmodel = new KDescendantsProxyModel(this);
	d->m_flatmodel->setDisplayAncestorData(true);
	d->m_flatmodel->setSourceModel(d->m_treemodel);
	setSourceModel(d->m_flatmodel);
	// fetch items
	QModelIndex i1 = index(0,0,QModelIndex());
	fetchMore(i1);
	emit collectionPopulated();
}

