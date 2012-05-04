/***************************************************************************
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

#include "playlistmodel.h"
#include <KDebug>

PlaylistModel::PlaylistModel(QObject* parent): QAbstractListModel(parent)
{
    m_currentIndex = -1;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        return m_musicList.at(index.row()).mediaName();
    }

    if (role == MediaCenter::MediaUrlRole) {
        return m_musicList.at(index.row()).mediaUrl();
    }

    return QVariant();
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    return m_musicList.count();
}

void PlaylistModel::addToPlaylist(const QString& url, const QString& name)
{
    beginResetModel();

    PlaylistItem item;
    item.setMediaName(name);
    item.setMediaUrl(url);
    m_musicList.append(item);

    endResetModel();
}

QString PlaylistModel::getNextUrl()
{
    m_currentIndex += 1;
    return m_musicList.at(m_currentIndex).mediaUrl();
}

int PlaylistModel::currentIndex()
{
    return m_currentIndex;
}

void PlaylistModel::setCurrentIndex(int index)
{
    m_currentIndex = index;
    emit currentIndexChanged();
}

QString PlaylistItem::mediaName() const
{
    return m_mediaName;
}

QString PlaylistItem::mediaUrl() const
{
    return m_mediaUrl;
}

void PlaylistItem::setMediaName(const QString name)
{
    m_mediaName = name;
}

void PlaylistItem::setMediaUrl(const QString url)
{
    m_mediaUrl =  url;
}
