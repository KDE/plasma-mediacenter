/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#include "eventsmodel.h"
#include <mediacenter/mediacenter.h>

#include <KGlobal>
#include <KSharedConfig>
#include <KConfigGroup>

namespace {
    static const char *CONFIG_GROUP = "eventsbackend.eventsmodel";
}

EventsModel::EventsModel(QObject* parent): QAbstractListModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    auto configGroup = KGlobal::config()->group(CONFIG_GROUP);
    QByteArray data = configGroup.readEntry("events", QByteArray());
    QDataStream s(&data, QIODevice::ReadOnly);

    s >> m_events;
    m_eventNames = m_events.keys();
}

QVariant EventsModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return m_eventNames.at(index.row());
    case MediaCenter::IsExpandableRole:
        return true;
    case Qt::DecorationRole:
        return "folder-image";
    }

    return QVariant();
}

int EventsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_eventNames.size();
}

void EventsModel::addEvent(const QString& eventName, const QDate& startDate, const QDate& endDate)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_eventNames.append(eventName);
    m_events.insert(eventName, QPair<QDate,QDate>(startDate, endDate));

    endInsertRows();

    saveEvents();
}

QPair< QDate, QDate > EventsModel::dateRangeForEvent(const QString& eventName) const
{
    return m_events.value(eventName);
}

void EventsModel::saveEvents()
{
    QByteArray data;
    QDataStream s(&data, QIODevice::ReadWrite);
    s << m_events;

    auto configGroup = KGlobal::config()->group(CONFIG_GROUP);
    configGroup.writeEntry("events", data);
    configGroup.sync();
}
