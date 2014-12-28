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

#include "modelsinbackendmodel.h"
#include "pmcmodel.h"

QHash< int, QByteArray > ModelsInBackendModel::roleNames() const
{
    auto defaultRoleNames = QAbstractItemModel::roleNames();
    defaultRoleNames.insert(ModelRole, "modelRole");
    return defaultRoleNames;
}

void ModelsInBackendModel::addModel(const PmcModel* model)
{
    auto rows = rowCount();
    beginInsertRows(QModelIndex(), rows, rows);
    m_models.append(model);
    endInsertRows();
}

bool ModelsInBackendModel::replaceModel(const PmcModel* original, const PmcModel* replacement)
{
    auto i = m_models.indexOf(original);
    if (i == -1) {
        return false;
    }
    m_models.replace(i, replacement);
    emit dataChanged(index(i), index(i));
    return true;
}

void ModelsInBackendModel::clear()
{
    beginResetModel();
    m_models.clear();
    endResetModel();
}

QVariant ModelsInBackendModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    switch (role) {
    case Qt::DisplayRole:
        return m_models.at(row)->name();
    case ModelRole:
        return QVariant::fromValue(m_models.at(row)->model());
    }
    return QVariant();
}

int ModelsInBackendModel::rowCount(const QModelIndex& parent) const
{
    Q_ASSERT(!parent.isValid());
    return m_models.size();
}
