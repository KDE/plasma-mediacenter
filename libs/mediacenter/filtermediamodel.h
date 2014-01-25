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

#ifndef FILTERMEDIAMODEL_H
#define FILTERMEDIAMODEL_H

#include <QString>
#include <QSortFilterProxyModel>

class FilterMediaModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    FilterMediaModel(QObject* parent = 0);
    ~FilterMediaModel();
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

    void setFilter(int role, const QVariant &filterValue);
    void addFilter(int role, const QVariant &filterValue);
    void clearFilters(bool invalidate = true);

private:
    QHash<int, QVariant> m_filters;
};

#endif // FILTERMEDIAMODEL_H
