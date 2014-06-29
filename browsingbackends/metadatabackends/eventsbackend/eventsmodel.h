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

#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include <QAbstractItemModel>
#include <QDate>

class EventsModel : public QAbstractListModel
{
public:
    explicit EventsModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void addEvent(const QString &eventName, const QDate &startDate, const QDate &endDate);
    QPair<QDate,QDate> dateRangeForEvent(const QString &eventName) const;

private:
    QList<QString> m_eventNames;
    QHash<QString, QPair<QDate, QDate>> m_events;
};

#endif // EVENTSMODEL_H
