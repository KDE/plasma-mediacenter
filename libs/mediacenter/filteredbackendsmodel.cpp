/***************************************************************************
 *   Copyright 2012 by Shantanu Tushar <shaan7in@gmail.com>                *
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

#include "filteredbackendsmodel.h"
#include "backendsmodel.h"

FilteredBackendsModel::FilteredBackendsModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

bool FilteredBackendsModel::filterAcceptsRow(int source_row, const QModelIndex& source_parent) const
{
    if (!sourceModel())
        return false;
    if (m_category.isEmpty())
        return true;
    return sourceModel()->data(sourceModel()->index(source_row, 0),
                               BackendsModel::BackendCategoryRole).toString()  == m_category;
}

void FilteredBackendsModel::setBackendCategory(const QString& category)
{
    m_category = category;
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

#include "filteredbackendsmodel.moc"
