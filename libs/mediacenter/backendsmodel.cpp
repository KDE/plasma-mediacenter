/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                               *
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

#include "backendsmodel.h"
#include "abstractbrowsingbackend.h"

#include <QDebug>
#include <KPluginTrader>

bool pluginLessThan(const KPluginInfo &lh, const KPluginInfo &rh)
{
    return lh.name().compare(rh.name(), Qt::CaseInsensitive) < 0;
}

class BackendsModel::Private
{
public:
    QHash<QString, MediaCenter::AbstractBrowsingBackend*> backends;
    KPluginInfo::List backendInfo;
    KPluginInfo::List loadedBackendsInfo;
};

BackendsModel::BackendsModel(QObject* parent)
    : QAbstractListModel(parent)
    , d(new Private)
{
    d->backendInfo = KPluginTrader::self()->query("plasma/mediacenter/browsingbackends");
    if (d->backendInfo.isEmpty()) {
        qWarning() << "no available browsing backend";
    }

    qStableSort(d->backendInfo.begin(), d->backendInfo.end(), pluginLessThan);

    QHash<int, QByteArray> roles = roleNames();
    roles[ModelObjectRole] = "modelObject";
    roles[BackendCategoryRole] = "backendCategory";
    setRoleNames(roles);

    loadBrowsingBackends();
}

void BackendsModel::loadBrowsingBackends()
{
    if (!d->backends.isEmpty())
        return;

    Q_FOREACH (KPluginInfo info, d->backendInfo) {

        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        const QVariantList args = QVariantList() << loader.metaData().toVariantMap();

        if(factory)
        {
            MediaCenter::AbstractBrowsingBackend *backend = factory->create<MediaCenter::AbstractBrowsingBackend>(0, args);
            if (backend) {
                backend->setPluginInfo(info);
                backend->setParent(const_cast<BackendsModel *>(this));
                d->backends.insert(info.libraryPath(), backend);
                d->loadedBackendsInfo.append(info);
            } else {
                qDebug() << "Could not create a instance for the backend " << info.name();
            }
        }

    }
}

QVariant BackendsModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    const KPluginInfo &info = d->loadedBackendsInfo.at(index.row());
    MediaCenter::AbstractBrowsingBackend *backend = d->backends.value(info.libraryPath());

    switch (role) {
        case Qt::DisplayRole:
            return backend->name();
        case Qt::DecorationRole:
            return backend->icon();
        case Qt::ToolTipRole:
            return backend->comment();
        case BackendCategoryRole:
            return backend->category();
        case ModelObjectRole:
            QVariant ptr;
            ptr.setValue(backend);
            return ptr;
    }

    return QVariant();
}

int BackendsModel::rowCount(const QModelIndex &) const
{
    return d->loadedBackendsInfo.count();
}
