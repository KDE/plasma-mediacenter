#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QtGui/QSortFilterProxyModel>
#include <kdescendantsproxymodel.h>

#include <Akonadi/Collection>

namespace Akonadi {
	class EntityTreeModel;
	class Session;
	class ChangeRecorder;
}

class KJob;


class ProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit ProxyModel( Akonadi::ChangeRecorder* monitor, Akonadi::Session* session, QObject* parent = 0 );
    virtual ~ProxyModel();
public:
	bool expand(int row);
	bool goOneLevelUp();
protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
private slots:
	void collPopulated(Akonadi::Collection::Id cid);
	void treemodelrdy();
signals:
	void collectionPopulated();
private:
	class Private;
	Private * const d;
};

#endif // PROXYMODEL_H
