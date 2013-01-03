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

    loadBrowsingBackends();
}

void BackendsModel::loadBrowsingBackends()
{
    if (!m_backends.isEmpty())
        return;

    Q_FOREACH (KPluginInfo info, m_backendInfo) {
        KService::Ptr service = info.service();
        if (!service) {
            kDebug() << "Could not get the service for the backend " << info.name();
            continue;
        }

        const QString key = service->library();
        QString errorMessage;
        MediaCenter::AbstractBrowsingBackend *backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId(), &errorMessage);
        if (backend) {
            if (!backend->okToLoad()) {
                kDebug() << "Backend " << info.name() << " doesn't want to be loaded";
                continue;
            }
            backend->setName(info.pluginName());
            backend->setParent(const_cast<BackendsModel *>(this));
            if (m_declarativeEngine) {
                backend->setDeclarativeEngine(m_declarativeEngine.data());
            }
            const_cast<BackendsModel *>(this)->m_backends.insert(key, backend);
            m_loadedBackendsInfo.append(info);
        } else {
            kDebug() << "Could not create a instance for the backend " << info.name() << errorMessage;
        }
    }
}

QVariant BackendsModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    const KPluginInfo &info = m_loadedBackendsInfo.at(index.row());
    switch (role) {
        case Qt::DisplayRole:
            return info.name();
        case Qt::DecorationRole:
            return info.icon();
        case Qt::ToolTipRole:
            return info.comment();
        case BackendCategoryRole:
            return info.category();
        case ModelObjectRole:
            QObject *backend = m_backends.value(info.service()->library());
            QVariant ptr;
            ptr.setValue(backend);
            return ptr;
    }

    return QVariant();
}

int BackendsModel::rowCount(const QModelIndex &) const
{
    return m_loadedBackendsInfo.count();
}

#include "backendsmodel.moc"
