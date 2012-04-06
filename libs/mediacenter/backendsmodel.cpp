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

#include "backendsmodel.h"

#include <libs/mediacenter/abstractbrowsingbackend.h>

#include <KDebug>

BackendsModel::BackendsModel (QDeclarativeEngine* engine, QObject* parent) : QAbstractListModel (parent)
{
    MediaCenter::AbstractBrowsingBackend *backend;
    KSharedPtr<KService> service;

    KService::List list = MediaCenter::AbstractBrowsingBackend::availableBackends();

    for (int i=0; i<list.size(); ++i) {
        service = list.at(i);
        backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId());
        loadBrowsingBackend(backend, engine);
    }

    QHash<int, QByteArray> roles = roleNames();
    roles[ModelObjectRole] = "modelObject";
    setRoleNames(roles);
}

bool BackendsModel::loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend* backend, QDeclarativeEngine* engine)
{
    if (!backend) {
        kDebug() << "OUCH! Something's wrong with the backend";
        return false;
    }

    backend->setParent(this);
    backend->setDeclarativeEngine(engine);

    if (!backend->okToLoad()) {
        kDebug() << "Backend " << backend->name() << " doesn't want to be loaded";
        return false;
    }

    backends.append(backend);
    return true;
}

QVariant BackendsModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole:
            return backends.at(index.row())->name();
        case Qt::DecorationRole:
            return backends.at(index.row())->icon();
        case ModelObjectRole:
            QVariant ptr;
            ptr.setValue(qobject_cast<QObject*>(backends.at(index.row())));
            return ptr;
    }

    return QVariant();
}

int BackendsModel::rowCount (const QModelIndex& parent) const
{
    return backends.count();
}

#include "backendsmodel.moc"
