#ifndef RSSMODEL_H
#define RSSMODEL_H

#include <QtCore/QModelIndex>

#include <KRss/FeedItemModel>


class RssModel : public KRss::FeedItemModel
{
	Q_OBJECT
public:
	explicit RssModel( Akonadi::ChangeRecorder* monitor, QObject* parent = 0 );
	~RssModel();
public:
	virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
};


#endif // RSSMODEL_H
