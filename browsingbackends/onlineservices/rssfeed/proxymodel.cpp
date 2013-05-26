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

#include "proxymodel.h"

#include <libs/mediacenter/mediacenter.h>
#include "rssmodel.h"
#include "feedcontroller.h"

#include <Akonadi/CachePolicy>
#include <Akonadi/Collection>
#include <Akonadi/ChangeRecorder>
#include <Akonadi/EntityTreeModel>
#include <Akonadi/Session>

#include <KRss/FeedCollection>

#include <QStack>


class ProxyModel::Private
{
public:
	Akonadi::ChangeRecorder* m_recorder;
	QStack<Akonadi::Collection>* m_parentcollection;
	QRegExp m_collectionfilter;
};

ProxyModel::ProxyModel ( Akonadi::ChangeRecorder* monitor, QObject* parent ) :
	QSortFilterProxyModel ( parent ),
	d( new Private() )
{
	setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
	KRss::FeedCollection::registerAttributes();
	d->m_recorder = monitor;

	d->m_parentcollection = new QStack<Akonadi::Collection>;
	d->m_parentcollection->push(monitor->collectionsMonitored().first());

	d->m_collectionfilter = QRegExp(d->m_parentcollection->top().remoteId(), Qt::CaseSensitive,
									QRegExp::FixedString);

    RssModel* rssmodel = new RssModel(d->m_recorder, this);
    KDescendantsProxyModel* flatmodel = new KDescendantsProxyModel(this);
    flatmodel->setDisplayAncestorData(true);
    flatmodel->setSourceModel(rssmodel);
    setSourceModel(flatmodel);
    // fetch items
    fetchMore(index(0,0,QModelIndex()));

    setFilterRole(Akonadi::EntityTreeModel::RemoteIdRole);
	setFilterRegExp(d->m_collectionfilter);
}

ProxyModel::~ProxyModel () {
	delete d;
}

bool ProxyModel::expand ( int row )
{
	KDescendantsProxyModel* m = qobject_cast<KDescendantsProxyModel*>(sourceModel());
    Akonadi::Collection selectedColl = mapToSource(index(row, 0)).data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
    d->m_parentcollection->push(selectedColl.parentCollection());
	d->m_collectionfilter.setPattern(selectedColl.remoteId());
	setFilterRegExp(d->m_collectionfilter);
	m->fetchMore(m->index(row, 0));
	return true;
}

bool ProxyModel::goOneLevelUp()
{
	if(d->m_parentcollection) {
		if(d->m_parentcollection->isEmpty()) {
			return false;
		}
		d->m_collectionfilter.setPattern(d->m_parentcollection->pop().remoteId());
		setFilterRegExp(d->m_collectionfilter);
		return true;
	}
	return false;
}

const Akonadi::Collection& ProxyModel::parent() const
{
	if (d->m_parentcollection) {
		return d->m_parentcollection->top();
	}
	return Akonadi::Collection();
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

