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

#ifndef PROXYMODEL_H
#define PROXYMODEL_H

#include <QtGui/QSortFilterProxyModel>
#include <kdescendantsproxymodel.h>

#include <Akonadi/Collection>

namespace Akonadi {
	class EntityTreeModel;
	class ChangeRecorder;
}

class KJob;


class ProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
	explicit ProxyModel( Akonadi::ChangeRecorder* monitor, QObject* parent = 0 );
    virtual ~ProxyModel();
public:
	bool expand(int row);
	bool goOneLevelUp();
public:
	const Akonadi::Collection& parent() const;
protected:
	bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
signals:
    void collectionTreeFetched(Akonadi::Collection::List colllist);
private:
	class Private;
	Private * const d;
};

#endif // PROXYMODEL_H
