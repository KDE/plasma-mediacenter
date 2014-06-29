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

#include <QDebug>

EventsModel::EventsModel(QObject* parent): QAbstractListModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
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
    qDebug() << "ADD " << startDate << endDate;
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    m_eventNames.append(eventName);
    m_events.insert(eventName, QPair<QDate,QDate>(startDate, endDate));

    endInsertRows();
}

QPair< QDate, QDate > EventsModel::dateRangeForEvent(const QString& eventName) const
{
    qDebug() << "FOR " << eventName << m_events.value(eventName);
    return m_events.value(eventName);
}
