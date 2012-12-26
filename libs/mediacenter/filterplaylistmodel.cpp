/***************************************************************************
 *   Copyright 2012 by Sujith Haridasan <sharidasan@kdemail.net>           *
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

#include "filterplaylistmodel.h"
#include <KDebug>


FilterPlaylistModel::FilterPlaylistModel(QObject* parent)
         : QSortFilterProxyModel(parent)
{

}

QString FilterPlaylistModel::filterString() const
{
    return m_searchString;
}

void FilterPlaylistModel::setFilterString(const QString& customString)
{
    beginResetModel();
    m_searchString = customString;
    endResetModel();
    emit filterStringChanged();
}

bool FilterPlaylistModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0 , sourceParent);
    return (sourceModel()->data(index).toString().contains(m_searchString, Qt::CaseInsensitive));
}

void FilterPlaylistModel::setSourcePlaylistModel(QObject* model)
{
    QAbstractItemModel *m = qobject_cast<QAbstractItemModel*>(model);
    if (m) {
        setSourceModel(m);
        emit sourcePlaylistModelChanged();
    }
}

QObject* FilterPlaylistModel::sourcePlaylistModel()
{
    return static_cast<QObject*>(sourceModel());
}

#include "filterplaylistmodel.moc"