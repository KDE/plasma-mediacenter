/***********************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                                *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef BACKENDSMODEL_H
#define BACKENDSMODEL_H

#include "kmediacollection_export.h"

#include <QAbstractItemModel>

#include <KPluginInfo>

namespace KMediaCollection {
    class AbstractBrowsingBackend;
}

class KMEDIACOLLECTION_EXPORT BackendsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        ModelObjectRole = Qt::UserRole + 1,
        BackendCategoryRole
    };
    explicit BackendsModel (QObject* parent = 0);
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

private:
    class Private;
    Private * const d;

    void loadBrowsingBackends();
};

Q_DECLARE_METATYPE(BackendsModel*)

#endif // BACKENDSMODEL_H
