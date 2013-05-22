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

#include "rssmodel.h"

#include <libs/mediacenter/mediacenter.h>

#include <KRss/Item>
#include <KRss/FeedCollection>
#include <KRss/Enclosure>


RssModel::RssModel ( Akonadi::ChangeRecorder* monitor, QObject* parent )
	: KRss::FeedItemModel( monitor, parent )
{
    QHash<int, QByteArray> roles(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    roles[KRss::FeedItemModel::HasFetchErrorRole] = "hasFetchError";
    roles[KRss::FeedItemModel::FetchErrorStringRole] = "fetchErrorString";
	setRoleNames(roles);
// 	setItemPopulationStrategy(Akonadi::EntityTreeModel::ImmediatePopulation);
}

RssModel::~RssModel()
{

}

QVariant RssModel::data ( const QModelIndex& index, int role ) const
{
	switch(role) {
		case Qt::DecorationRole:
        {
            const Akonadi::Collection c = KRss::FeedItemModel::data(index, Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>();
            const KRss::FeedCollection coll(c);
            if (coll.imageUrl().isEmpty()) {
                return QString();
            }
            return coll.imageUrl();
        }
		case MediaCenter::MediaUrlRole:
		{
			const Akonadi::Item akonadiItem = KRss::FeedItemModel::data(index, Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();

			if( !akonadiItem.hasPayload<KRss::Item>() ) {
				return KRss::FeedItemModel::data( index, role );
            }

			const KRss::Item item = akonadiItem.payload<KRss::Item>();
			QList<KRss::Enclosure> media = item.enclosures();
			// TODO handle multiple media links
            if (!media.isEmpty()) {
                return media.first().url();
            }
            return QString();
		}
		case MediaCenter::MediaTypeRole:
			return "video";
		case MediaCenter::IsExpandableRole:
		{
			return index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>().isValid();
		}
		default:
        {
			return KRss::FeedItemModel::data( index, role );
        }
	}
}


