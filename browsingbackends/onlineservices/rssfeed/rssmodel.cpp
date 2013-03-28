
#include "rssmodel.h"

#include <libs/mediacenter/mediacenter.h>

#include <KRss/Item>
#include <KRss/Enclosure>


RssModel::RssModel ( Akonadi::ChangeRecorder* monitor, QObject* parent )
	: KRss::FeedItemModel( monitor, parent )
{
	setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

RssModel::~RssModel()
{

}

QVariant RssModel::data ( const QModelIndex& index, int role ) const
{
	switch(role) {
		case Qt::DecorationRole:
			//TODO
			return "http://www.jupiterbroadcasting.com/jupiter.png";
		case MediaCenter::MediaUrlRole:
		{
			const Akonadi::Item akonadiItem = KRss::FeedItemModel::data(index, Akonadi::EntityTreeModel::ItemRole).value<Akonadi::Item>();

			if( !akonadiItem.hasPayload<KRss::Item>() )
				return KRss::FeedItemModel::data( index, role );

			const KRss::Item item = akonadiItem.payload<KRss::Item>();
			QList<KRss::Enclosure> media = item.enclosures();
			// TODO handle multiple media links
			return media.first().url();
		}
		case MediaCenter::MediaTypeRole:
			return "video";
		case MediaCenter::IsExpandableRole:
		{
			return index.data(Akonadi::EntityTreeModel::CollectionRole).value<Akonadi::Collection>().isValid();
		}
		default:
			return KRss::FeedItemModel::data( index, role );
	}
}


