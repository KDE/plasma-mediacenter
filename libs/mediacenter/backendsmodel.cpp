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

#include <QDeclarativeEngine>

#include <KDebug>

#include <libs/mediacenter/abstractbrowsingbackend.h>

bool pluginLessThan(const KPluginInfo &lh, const KPluginInfo &rh)
{
    return lh.name().compare(rh.name(), Qt::CaseInsensitive) < 0;
}

BackendsModel::BackendsModel(QDeclarativeEngine *engine, QObject* parent)
    : QAbstractListModel(parent),
      m_declarativeEngine(engine)
{
    m_declarativeEngine = engine;
    KService::List services = MediaCenter::AbstractBrowsingBackend::availableBackends();
    m_backendInfo = KPluginInfo::fromServices(services);
    qStableSort(m_backendInfo.begin(), m_backendInfo.end(), pluginLessThan);

    QHash<int, QByteArray> roles = roleNames();
    roles[ModelObjectRole] = "modelObject";
    roles[BackendCategoryRole] = "backendCategory";
    setRoleNames(roles);
}

MediaCenter::AbstractBrowsingBackend *BackendsModel::loadBrowsingBackend(const KPluginInfo &info) const
{
    KService::Ptr service = info.service();
    if (!service) {
        return 0;
    }

    const QString key = service->library();
    MediaCenter::AbstractBrowsingBackend *backend = m_backends.value(key);
    if (backend) {
        return backend;
    }

    // no? well then, let's load it.
    backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId());
    const_cast<BackendsModel *>(this)->m_backends.insert(key, backend);

    if (backend) {
        backend->setName(info.pluginName());

        if (backend->okToLoad()) {
            backend->setParent(const_cast<BackendsModel *>(this));
            if (m_declarativeEngine) {
                backend->setDeclarativeEngine(m_declarativeEngine.data());
            }
        } else {
            kDebug() << "Backend " << info.name() << " doesn't want to be loaded";
        }
    } else {
        kDebug() << "OUCH! Something's wrong with the backend";
    }

    return backend;
}

QVariant BackendsModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    const KPluginInfo &info = m_backendInfo.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return info.name();
        case Qt::DecorationRole:
            return info.icon();
        case BackendCategoryRole:
            return info.category();
        case ModelObjectRole:
            QObject *backend = loadBrowsingBackend(info);
            QVariant ptr;
            ptr.setValue(backend);
            return ptr;
    }

    return QVariant();
}

int BackendsModel::rowCount(const QModelIndex &) const
{
    return m_backendInfo.count();
}

#include "backendsmodel.moc"
