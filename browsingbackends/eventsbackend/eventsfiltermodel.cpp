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

#include "eventsfiltermodel.h"
#include <mediacenter.h>

#include <QDateTime>
#include <QDebug>

EventsFilterModel::EventsFilterModel(QObject* parent): FilterMediaModel(parent)
{

}

void EventsFilterModel::setDateRange(const QString& eventName, const QDate& startDate, const QDate& endDate)
{
    m_eventName = eventName;
    m_startDate = startDate;
    m_endDate = endDate;

    invalidateFilter();
}

QPair< QDate, QDate > EventsFilterModel::dateRange() const
{
    return QPair<QDate,QDate>(m_startDate, m_endDate);
}

QString EventsFilterModel::eventName() const
{
    return m_eventName;
}

bool EventsFilterModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    Q_UNUSED(source_parent);
    if(!sourceModel()) {
        return false;
    }
    if (!m_startDate.isValid() || !m_endDate.isValid()) {
        return true;
    }

    const QModelIndex index = sourceModel()->index(source_row, 0);
    const QDate date = sourceModel()->data(index, MediaCenter::CreatedAtRole).toDateTime().date();

    return (date > m_startDate && date < m_endDate);
}
