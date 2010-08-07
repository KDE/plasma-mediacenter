/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "backendmodel.h"

#include <mediacenter/abstractbrowsingbackend.h>

#include <KIcon>
#include <KServiceTypeTrader>
#include <KService>
#include <KSycoca>
#include <KDebug>

BackendModel::BackendModel(QObject *parent) : QAbstractItemModel(parent)
{
}

BackendModel::~BackendModel()
{}

int BackendModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }

    return 0;
}

int BackendModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_modelServices.count();
    }

    return 0;
}

QVariant BackendModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount() || index.column() >= columnCount()) {
        return QVariant();
    }

    QVariant data;
    if (role == Qt::DisplayRole) {
        const KService *service = static_cast<KService*>(index.internalPointer());
        data = service->name();
    } else if (role == Qt::DecorationRole) {
        const KService *service = static_cast<KService*>(index.internalPointer());
        data = KIcon(service->icon());
    } else if (role == BackendModel::ServiceRole) {
        data = QVariant::fromValue(static_cast<KService*>(index.internalPointer()));
    }

    return data;
}

QModelIndex BackendModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return QModelIndex();
    }

    if (row >= rowCount() || column >= columnCount()) {
        return QModelIndex();
    }

    KService::Ptr servicePtr = m_modelServices.value(row);
    return createIndex(row, column, (void*)servicePtr.constData());
}

QModelIndex BackendModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

void BackendModel::setModelServices(const KService::List &services)
{
    m_modelServices = services;
    reset();
}
