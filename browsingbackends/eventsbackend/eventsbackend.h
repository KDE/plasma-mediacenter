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

#ifndef EVENTSBACKEND_H
#define EVENTSBACKEND_H

#include "../abstractmetadatabackend.h"

class ModelMetadata;
class EventsFilterModel;
class EventsPicturesModel;
class EventsModel;
class EventsBackend : public AbstractMetadataBackend
{
    Q_OBJECT
public:
    EventsBackend(QObject* parent, const QVariantList& args);
    virtual void handleButtonClick(const QString& button);
    virtual bool expand(int row);
    virtual bool goOneLevelUp();

    Q_INVOKABLE QList<int> editingStart() const;
    Q_INVOKABLE QList<int> editingEnd() const;
    Q_INVOKABLE QString editingEventName() const;

public Q_SLOTS:
    QString addEvent(const QDate &startDate, const QDate &endDate, const QString &eventName);
    QString editEvent(const QDate &startDate, const QDate &endDate, const QString &eventName);

    bool closeEventsConfiguration();

protected:
    virtual bool initImpl();

private Q_SLOTS:
    void showConfiguration();

private:
    QString m_loginText;
    ModelMetadata *m_eventsModelMetadata = nullptr;
    EventsModel *m_eventsModel = nullptr;
    EventsPicturesModel *m_eventsPicturesModel = nullptr;
    EventsFilterModel *m_eventsFilterModel = nullptr;
    ModelMetadata *m_eventsFilterModelMetadata = nullptr;

    QDate m_editingStartDate;
    QDate m_editingEndDate;
    QString m_editingEventName;

    QString eventNameForRow(int row) const;
};

#endif // EVENTSBACKEND_H
