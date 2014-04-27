/***********************************************************************************
 *  Copyright 2012 by Sujith Haridasan <sujith.haridasan@kdemail.net>              *
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

#include "filterplaylistmodel.h"
#include "playlistmodel.h"
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
    m_searchString = customString;
    invalidateFilter();
    emit filterStringChanged();
}

bool FilterPlaylistModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0 , sourceParent);
    return sourceModel()->data(index).toString().contains(m_searchString, Qt::CaseInsensitive)
        || sourceModel()->data(index, PlaylistModel::MediaArtistRole).toString().contains(m_searchString, Qt::CaseInsensitive);
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

int FilterPlaylistModel::currentIndex() const
{
    PlaylistModel *playlistmodel = static_cast<PlaylistModel*>(sourceModel());
    return mapFromSource(sourceModel()->index(playlistmodel->currentIndex(), 0)).row();
}

void FilterPlaylistModel::setCurrentIndex(int presentIndex)
{
    QModelIndex i = index(presentIndex, 0);
    PlaylistModel *playlistmodel = static_cast<PlaylistModel*>(sourceModel());
    playlistmodel->setCurrentIndex(mapToSource(i).row());
    emit currentIndexChanged();
}

void FilterPlaylistModel::removeFromSourceModel(int indexInFilterModel)
{
    PlaylistModel *playlistmodel = static_cast<PlaylistModel*>(sourceModel());
    playlistmodel->removeFromPlaylist(mapToSource(index(indexInFilterModel, 0)).row());
}

void FilterPlaylistModel::playFromSourceModel(int indexInFilterModel)
{
    PlaylistModel *playlistmodel = static_cast<PlaylistModel*>(sourceModel());
    playlistmodel->play(mapToSource(index(indexInFilterModel, 0)).row());
}
