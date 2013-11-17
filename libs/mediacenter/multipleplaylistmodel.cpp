/***********************************************************************************
 *   Copyright 2013 by Sinny Kumari <ksinny@gmail.com>                             *
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

#include "multipleplaylistmodel.h"

#include <KDE/KStandardDirs>
#include <KDE/KCmdLineArgs>
#include <QDebug>
#include <QDir>

MultiplePlaylistModel::MultiplePlaylistModel(QObject* parent): QAbstractListModel(parent)
{
    QString dirPath = KGlobal::dirs()->saveLocation("data") + KCmdLineArgs::appName() + "/playlists/";
    QDir dir(dirPath);
    QStringList playlistList = dir.entryList (QDir::NoDotAndDotDot | QDir::Files);
    foreach (QString str, playlistList) {
        m_multiplePlaylistList.append(str);
    }
    if (playlistList.length() == 0) {
        m_multiplePlaylistList.append("Default");
    }
}

MultiplePlaylistModel::~MultiplePlaylistModel()
{

}

int MultiplePlaylistModel::rowCount(const QModelIndex& parent) const
{

    return m_multiplePlaylistList.size();
}


QVariant MultiplePlaylistModel::data(const QModelIndex& index, int role) const
{
    switch(role) {
        case Qt::DisplayRole:
            return m_multiplePlaylistList.at(index.row());
    }
    
    return QVariant();
    
}

void MultiplePlaylistModel::createNewPlaylist(const QString& name)
{
    const int n = rowCount();
    beginInsertRows(QModelIndex(), n, n);
    m_multiplePlaylistList.append(name);
    endInsertRows();
    m_playlistModel->setNewPlaylist(name);
}

void MultiplePlaylistModel::setPlaylistModelAddress(QObject* model)
{
    if (model != NULL) {
        m_playlistModel = qobject_cast<PlaylistModel *> (model);
    }
}

void MultiplePlaylistModel::switchToPlaylist(QString name)
{
    m_playlistModel->switchToPlaylist(name);
}

void MultiplePlaylistModel::removeCurrentPlaylist()
{
    beginResetModel();
    QString currPlaylist = m_playlistModel->playlistName();
    bool status = m_playlistModel->removeCurrentPlaylist();
    if (status) {
        m_multiplePlaylistList.removeOne (currPlaylist);
    }
    endResetModel();
}

