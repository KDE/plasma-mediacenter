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

#ifndef EVENTSFILTERMODEL_H
#define EVENTSFILTERMODEL_H

#include <mediacenter/filtermediamodel.h>
#include <QDate>

class EventsFilterModel : public FilterMediaModel
{
public:
    explicit EventsFilterModel(QObject* parent = 0);

    void setDateRange(const QString &eventName, const QDate &startDate, const QDate &endDate);
    QString eventName() const;
    QPair<QDate,QDate> dateRange() const;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

private:
    QString m_eventName;
    QDate m_startDate;
    QDate m_endDate;
};

#endif // EVENTSFILTERMODEL_H
