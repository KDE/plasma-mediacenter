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
    if (m_searchText == "") {
        return true;
    }

    return sourceModel()->data(sourceModel()->index(source_row, 0),
                               Qt::DisplayRole).toString().contains(m_searchText,Qt::CaseInsensitive);
}

void FilterMediaModel::setSearchText(const QString& text)
{
    beginResetModel();
    m_searchText = text;
    endResetModel();
}

