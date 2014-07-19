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

#include "eventsbackend.h"
#include "eventspicturesmodel.h"
#include "eventsfiltermodel.h"
#include "eventsmodel.h"

#include <mediacenter/filtermediamodel.h>
#include <mediacenter/modelmetadata.h>

#include <QTimer>

MEDIACENTER_EXPORT_BROWSINGBACKEND(EventsBackend)

namespace {
    const QString s_addEventButton = i18n("Add event or trip");
    const QString s_editEventButton = i18n("Edit");
    const QString s_deleteEventButton = i18n("Delete");
}

EventsBackend::EventsBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
{
    m_editingStartDate = QDate::currentDate();
    m_editingEndDate = QDate::currentDate();
}

bool EventsBackend::initImpl()
{
    m_eventsModel = new EventsModel(this);
    m_eventsModelMetadata = new ModelMetadata(m_eventsModel, this);

    m_eventsPicturesModel = new EventsPicturesModel(this);

    m_eventsFilterModel = new EventsFilterModel(this);
    m_eventsFilterModel->setSourceModel(m_eventsPicturesModel);
    m_eventsFilterModel->setSortRole(MediaCenter::CreatedAtRole);
    m_eventsFilterModel->sort(0, Qt::DescendingOrder);
    m_eventsFilterModelMetadata = new ModelMetadata(m_eventsFilterModel, this);

    setModel(m_eventsModelMetadata);
    setButtons(QStringList() << s_addEventButton);
    return true;
}

void EventsBackend::showConfiguration()
{
    emit showCustomUi(constructQmlSource("eventscomponents", "0.1", "EventsConfiguration"));
}

void EventsBackend::handleButtonClick(const QString& button)
{
    if (button == s_addEventButton) {
        m_editingEventName = QString();
        showConfiguration();
    } else if (button == s_editEventButton) {
        auto editingDateRange = m_eventsFilterModel->dateRange();
        m_editingStartDate = editingDateRange.first;
        m_editingEndDate = editingDateRange.second;
        m_editingEventName = m_eventsFilterModel->eventName();

        showConfiguration();
    } else if (button == s_deleteEventButton) {
        if (m_eventsModel->deleteEvent(m_eventsFilterModel->eventName())) {
            goOneLevelUp();
        }
    }
}

bool EventsBackend::addEvent(int dayStart, int monthStart, int yearStart, int dayEnd, int monthEnd, int yearEnd, const QString& eventName)
{
    auto dateStart = QDate(yearStart, monthStart, dayStart);
    auto dateEnd = QDate(yearEnd, monthEnd, dayEnd);

    if (m_eventsModel->addOrEditEvent(eventName, dateStart, dateEnd)) {
        if (model() == m_eventsFilterModel) {
            auto dateRange = m_eventsModel->dateRangeForEvent(eventName);
            m_eventsFilterModel->setDateRange(eventName, dateRange.first, dateRange.second);
        }
        return true;
    }

    return false;
}

bool EventsBackend::expand(int row)
{
    auto eventName = eventNameForRow(row);

    auto dateRange = m_eventsModel->dateRangeForEvent(eventName);
    m_eventsFilterModel->setDateRange(eventName, dateRange.first, dateRange.second);
    setModel(m_eventsFilterModelMetadata);

    setButtons(QStringList() << s_editEventButton << s_deleteEventButton);

    return true;
}

bool EventsBackend::goOneLevelUp()
{
    if (model() == m_eventsModel) {
        return false;
    }

    setModel(m_eventsModelMetadata);
    setButtons(QStringList() << s_addEventButton);
    return true;
}

QList< int > EventsBackend::editingStart() const
{
    return QList<int>() << m_editingStartDate.day() << m_editingStartDate.month() << m_editingStartDate.year();
}

QList< int > EventsBackend::editingEnd() const
{
    return QList<int>() << m_editingEndDate.day() << m_editingEndDate.month() << m_editingEndDate.year();
}

QString EventsBackend::editingEventName() const
{
    return m_editingEventName;
}

QString EventsBackend::eventNameForRow(int row) const
{
    return m_eventsModel->data(m_eventsModel->index(row), Qt::DisplayRole).toString();
}

bool EventsBackend::closeEventsConfiguration()
{
    return false;
}
