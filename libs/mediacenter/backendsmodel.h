/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef BACKENDSMODEL_H
#define BACKENDSMODEL_H

#include "mediacenter_export.h"

#include <QAbstractItemModel>

class QDeclarativeEngine;

namespace MediaCenter {
    class AbstractBrowsingBackend;
}

class MEDIACENTER_EXPORT BackendsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        ModelObjectRole = Qt::UserRole + 1,
        BackendCategoryRole
    };

    explicit BackendsModel (QDeclarativeEngine *engine, QObject* parent = 0);
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

private:
    bool loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *backend, QDeclarativeEngine *engine);

    QList<MediaCenter::AbstractBrowsingBackend*> backends;
};

#endif // BACKENDSMODEL_H
