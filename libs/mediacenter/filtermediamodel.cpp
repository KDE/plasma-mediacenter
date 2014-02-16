/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#include "filtermediamodel.h"

FilterMediaModel::FilterMediaModel(QObject* parent): QSortFilterProxyModel(parent)
{
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
}

FilterMediaModel::~FilterMediaModel()
{

}

bool FilterMediaModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    Q_UNUSED(source_parent);
    if(!sourceModel()) {
        return false;
    }

    if (m_filters.keys().isEmpty()) {
        return true;
    }

    const QModelIndex index = sourceModel()->index(source_row, 0);

    Q_FOREACH (int role, m_filters.keys()) {
        const QVariant value = m_filters.value(role);
        if (value.canConvert(QVariant::String)) {
            if (!sourceModel()->data(index, role).toString().contains(
                value.toString(), sortCaseSensitivity())) {
                return false;
            }
        }
    }

    return true;
}

void FilterMediaModel::addFilter(int role, const QVariant& filterValue)
{
    beginResetModel();
    m_filters.insert(role, filterValue);
    endResetModel();
}

void FilterMediaModel::clearFilters(bool invalidate)
{
    if (invalidate) beginResetModel();
    m_filters.clear();
    if (invalidate) endResetModel();
}

void FilterMediaModel::setFilter(int role, const QVariant& filterValue)
{
    clearFilters(false);
    addFilter(role, filterValue);
}
