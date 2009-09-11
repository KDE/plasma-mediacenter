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
#include "startupmodel.h"

#include <KIcon>
#include <KService>
#include <KServiceTypeTrader>
#include <KSycoca>
#include <KDebug>

StartupModel::StartupModel(QObject *parent) : QAbstractItemModel(parent)
{
    init();
    connect (KSycoca::self(), SIGNAL(databaseChanged(QStringList)), this, SLOT(updateModel(QStringList)));
}

StartupModel::~StartupModel()
{}

void StartupModel::init()
{
    KService::List plugins = KServiceTypeTrader::self()->query("Plasma/MediaCenter/ModelPackage");
    if (plugins.isEmpty()) {
        kError() << "no available model package";
        return;
    }

    addAvailableModels(plugins);
}

int StartupModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 1;
    }

    return 0;
}

int StartupModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return m_modelServices.count();
    }

    return 0;
}

QVariant StartupModel::data(const QModelIndex &index, int role) const
{
    if (index.parent().isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount() || index.column() >= columnCount()) {
        return QVariant();
    }

    switch (role) {
        case Qt::DisplayRole :
        return m_modelServices[index.row()]->name();
        break;
        case Qt::DecorationRole :
        return KIcon(m_modelServices[index.row()]->icon());
        break;
        default :
                return QVariant();
                ;
    }
}

QModelIndex StartupModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return QModelIndex();
    }

    if (row >= rowCount() || column >= columnCount()) {
        return QModelIndex();
    }

    return createIndex(row, column, m_modelServices[row]);
}

QModelIndex StartupModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

void StartupModel::addAvailableModels(const KService::List &models)
{
    beginInsertRows(QModelIndex(), 0, models.count() - 1);
    m_modelServices << models;
    endInsertRows();
}

void StartupModel::updateModel(const QStringList &changedResources)
{
    if (!changedResources.contains("services")) {
        return;
    }

    beginRemoveRows(QModelIndex(), 0, m_modelServices.count() -1);
    m_modelServices.clear();
    endRemoveRows();

    init();
}
