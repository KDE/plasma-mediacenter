/***********************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
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

#include "filteredbackendsmodel.h"
#include "backendsmodel.h"

FilteredBackendsModel::FilteredBackendsModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool FilteredBackendsModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent)

    if (!sourceModel()) {
        return false;
    }

    if (m_category.isEmpty()) {
        return true;
    }

    return sourceModel()->data(sourceModel()->index(source_row, 0),
                               BackendsModel::BackendCategoryRole).toString() == m_category;
}

void FilteredBackendsModel::setBackendCategory(const QString& category)
{
    beginResetModel();
    m_category = category;
    endResetModel();
    emit backendCategoryChanged();
}

QString FilteredBackendsModel::backendCategory() const
{
    return m_category;
}

void FilteredBackendsModel::setSourceBackendsModel(QObject* model)
{
    QAbstractItemModel *m = qobject_cast<QAbstractItemModel*>(model);
    if (m) {
        setSourceModel(m);
    }
    emit sourceBackendsModelChanged();
}

QObject* FilteredBackendsModel::sourceBackendsModel()
{
    return static_cast<QObject*>(sourceModel());
}
