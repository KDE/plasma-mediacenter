/***********************************************************************************
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                             *
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

#include "playlistmodel.h"
#include "playlistitem.h"

#include <QDebug>

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>

namespace {
    static const char DEFAULT_PLAYLIST_NAME[] = "Default";
}

class PlaylistModel::Private
{
public:
    QList<PlaylistItem*> musicList;
    int currentIndex;
    QFile file;
    bool random;
    QString playlistName;
    QString playlistsDirectoryPath;
    bool cmdLineURL;
};

PlaylistModel::PlaylistModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private)
{
    d->playlistName = DEFAULT_PLAYLIST_NAME;
    loadFromFile(playlistFilePath());

    d->currentIndex = -1;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    QHash<int, QByteArray> newRoles(roleNames());
    newRoles[MediaLengthRole] = "mediaLength";
    newRoles[MediaArtistRole] = "mediaArtist";
    newRoles[OriginalIndexRole] = "originalIndex";
    setRoleNames(newRoles);

    qsrand(QDateTime::currentMSecsSinceEpoch());

    connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), SLOT(savePlaylist()));

    connect(this, SIGNAL(currentIndexChanged()), SIGNAL(currentUrlChanged()));
}

PlaylistModel::~PlaylistModel()
{
    savePlaylist();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= rowCount()) {
        return QVariant();
    }

    switch(role) {
    case Qt::DisplayRole:
        return d->musicList.at(index.row())->mediaName();
    case MediaCenter::MediaUrlRole:
        return d->musicList.at(index.row())->mediaUrl();
    case PlaylistModel::MediaArtistRole:
        return d->musicList.at(index.row())->mediaArtist();
    case PlaylistModel::MediaLengthRole:
        return d->musicList.at(index.row())->mediaLength();
    case PlaylistModel::OriginalIndexRole:
        return index.row();
    }
    return QVariant();
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return d->musicList.size();
}

int PlaylistModel::addToPlaylist(const QStringList& urls)
{
    const int n = rowCount();
    if (urls.isEmpty()) return n-1;

    beginInsertRows(QModelIndex(), n, n+urls.size()-1);

    Q_FOREACH(const QString &url, urls) {
        PlaylistItem *item = new PlaylistItem(url, this);
        connect(item, SIGNAL(updated()), SLOT(playlistItemUpdated()));
        d->musicList.append(item);
    }

    endInsertRows();

    return n;
}

void PlaylistModel::addToPlaylist(const QString& url)
{
    addToPlaylist(QStringList() << url);
}

void PlaylistModel::removeFromPlaylist(const int& index)
{
    beginRemoveRows(QModelIndex(), index, index);
    d->musicList.takeAt(index)->deleteLater();
    if (index <= d->currentIndex) {
        d->currentIndex -= 1;
    }
    endRemoveRows();
}

void PlaylistModel::moveItem(int originalIndex, int newIndex)
{
    int moveBeforeRow = (newIndex > originalIndex) ? newIndex + 1 : newIndex;

    beginMoveRows(QModelIndex(), originalIndex, originalIndex, QModelIndex(), moveBeforeRow);
    d->musicList.move(originalIndex, newIndex);
    endMoveRows();

    if (originalIndex == d->currentIndex) {
        setCurrentIndex(newIndex);
    } else if (originalIndex < d->currentIndex && newIndex >= d->currentIndex) {
        setCurrentIndex(d->currentIndex-1);
    } else if (originalIndex > d->currentIndex && newIndex <= d->currentIndex) {
        setCurrentIndex(d->currentIndex+1);
    }
}

void PlaylistModel::playNext()
{
    if (d->currentIndex == d->musicList.count() - 1) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(d->currentIndex + 1);
    }

    play(currentIndex());
}

void PlaylistModel::playPrevious()
{
    if (d->currentIndex == 0) {
        setCurrentIndex(d->musicList.count() - 1);
    } else {
        setCurrentIndex(d->currentIndex - 1);
    }

    play(currentIndex());
}

QString PlaylistModel::getUrlofFirstIndex()
{
    return d->musicList.isEmpty() ? QString() : d->musicList.at(0)->mediaUrl();
}

void PlaylistModel::clearPlaylist()
{
    beginResetModel();
    clearPlaylistWithoutModelReset();
    endResetModel();
}

void PlaylistModel::clearPlaylistWithoutModelReset()
{
    Q_FOREACH(PlaylistItem *item, d->musicList) {
        item->deleteLater();
    }

    d->musicList.clear();
    d->currentIndex = -1;
}

int PlaylistModel::currentIndex() const
{
    return d->currentIndex;
}

void PlaylistModel::setCurrentIndex(int index)
{
    d->currentIndex = index;
    emit currentIndexChanged();
}

void PlaylistModel::shuffle()
{
    if( d->musicList.isEmpty())
        return;

    QList<PlaylistItem*> musicListShuffle;
    if( d->currentIndex == -1 )
        d->currentIndex = 0;
    musicListShuffle.append(d->musicList.takeAt(d->currentIndex));

    while( !d->musicList.isEmpty() ) {
        musicListShuffle.append(d->musicList.takeAt(qrand() % d->musicList.size()));
    }

    beginResetModel();
    d->musicList = musicListShuffle;
    endResetModel();
    setCurrentIndex(0);
}

void PlaylistModel::playlistItemUpdated()
{
    PlaylistItem *item = qobject_cast<PlaylistItem*>(sender());

    int i = d->musicList.indexOf(item);
    emit dataChanged(createIndex(i, 0), createIndex(i, 0));
}

QString PlaylistModel::playlistFilePath() const
{
    return  getPlaylistPath() + d->playlistName;
}

void PlaylistModel::loadFromFile(const QString& path)
{
    if (QFile::exists(path)) {
        QFile file(path);
        if (file.open(QIODevice::ReadOnly)) {
            QDomDocument doc;
            doc.setContent(file.readAll());
            file.close();

            QDomNodeList itemList = doc.elementsByTagName("item");
            d->musicList.clear();
            setCurrentIndex(-1);
            for (int i=0; i<itemList.count(); i++) {
                QDomNode node = itemList.at(i);
                if (node.isNull()) continue;
                const QString url = node.toElement().attribute("url");
                if (url.isEmpty()) continue;

                PlaylistItem *item = new PlaylistItem(url, this);
                connect(item, SIGNAL(updated()), SLOT(playlistItemUpdated()));
                d->musicList.append(item);
            }
        }
    }
}

void PlaylistModel::saveToFile(const QString& path) const
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QDomDocument doc;
        QDomElement playlist = doc.createElement("playlist");

        Q_FOREACH (const PlaylistItem *item, d->musicList) {
            QDomElement element = doc.createElement("item");
            element.setAttribute("url", item->mediaUrl());
            playlist.appendChild(element);
        }

        doc.appendChild(playlist);
        QTextStream s(&file);
        doc.save(s, 0);
        file.close();
    }
}

void PlaylistModel::savePlaylist()
{
    saveToFile(playlistFilePath());
}

bool PlaylistModel::removeCurrentPlaylist(const QString &playlistToSwitchToAfterDeletion)
{
    if (d->playlistName == DEFAULT_PLAYLIST_NAME) {
        clearPlaylist();
        return false;
    } else {
        beginResetModel();

        QFile::remove(playlistFilePath());
        d->playlistName = playlistToSwitchToAfterDeletion;
        loadFromFile (playlistFilePath());

        endResetModel();
        return true;
    }
}


QString PlaylistModel::playlistName() const
{
    return d->playlistName;
}

void PlaylistModel::setPlaylistName(const QString& name)
{
    if (playlistName() == name) return;

    beginResetModel();
    saveToFile(playlistFilePath());

    clearPlaylistWithoutModelReset();
    d->playlistName = name;

    loadFromFile (playlistFilePath());
    endResetModel();
}

QString PlaylistModel::getPlaylistPath() const
{
    if (d->playlistsDirectoryPath.isEmpty()) {
        d->playlistsDirectoryPath = MediaCenter::dataDirForComponent("playlists");
        QDir().mkpath(d->playlistsDirectoryPath);
    }
    return d->playlistsDirectoryPath;
}

bool PlaylistModel::processCommandLineArgs(const QStringList* urls)
{
    if (urls->size()) {
        setPlaylistName(DEFAULT_PLAYLIST_NAME);
        const int indexOfFirstMedia = addToPlaylist(*urls);

        setCurrentIndex(indexOfFirstMedia);
        return true;
    }

    return false;
}

void PlaylistModel::play(int index)
{
    setCurrentIndex(index);
}

QString PlaylistModel::currentUrl() const
{
    return data(index(currentIndex()), MediaCenter::MediaUrlRole).toString();
}
