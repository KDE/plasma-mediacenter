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

#include <KDebug>
#include <KDE/KStandardDirs>
#include <KDE/KCmdLineArgs>

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>

class PlaylistModel::Private
{
public:
    QList<PlaylistItem*> musicList;
    int currentIndex;
    QFile file;
    bool random;
};

PlaylistModel::PlaylistModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private)
{
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
}

PlaylistModel::~PlaylistModel()
{
    savePlaylist();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
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
    return d->musicList.count();
}

void PlaylistModel::addToPlaylist(const QString& url)
{
    const int n = rowCount();
    beginInsertRows(QModelIndex(), n, n);

    PlaylistItem *item = new PlaylistItem(url, this);
    connect(item, SIGNAL(updated()), SLOT(playlistItemUpdated()));
    d->musicList.append(item);

    endInsertRows();
}

void PlaylistModel::removeFromPlaylist(const int& index)
{
    beginResetModel();
    d->musicList.takeAt(index)->deleteLater();
    if (index <= d->currentIndex) {
        d->currentIndex -= 1;
    }
    endResetModel();
}

void PlaylistModel::moveItem(int firstIndex, int secondIndex)
{
    d->musicList.move(firstIndex,secondIndex);
    emit dataChanged(createIndex(firstIndex, 0), createIndex(secondIndex, 0));
}

QString PlaylistModel::getNextUrl()
{
    if (d->currentIndex == d->musicList.count() - 1) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(d->currentIndex + 1);
    }
    return d->musicList.at(d->currentIndex)->mediaUrl();
}

QString PlaylistModel::getPreviousUrl()
{
    if (d->currentIndex == 0) {
        setCurrentIndex(d->musicList.count() - 1);
    } else {
        setCurrentIndex(d->currentIndex - 1);
    }
    return d->musicList.at(d->currentIndex)->mediaUrl();
}

QString PlaylistModel::getUrlofFirstIndex()
{
    return d->musicList.isEmpty() ? QString() : d->musicList.at(0)->mediaUrl();
}

void PlaylistModel::clearPlaylist()
{
    beginResetModel();

    Q_FOREACH(PlaylistItem *item, d->musicList) {
        item->deleteLater();
    }

    d->musicList.clear();
    d->currentIndex = -1;

    endResetModel();
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
    QList<PlaylistItem*> musicListShuffle;
    while( !d->musicList.isEmpty() ) {
        musicListShuffle.append(d->musicList.takeAt(qrand() % d->musicList.size()));
    }
    beginResetModel();
    d->musicList = musicListShuffle;
    endResetModel();
}


void PlaylistModel::playlistItemUpdated()
{
    PlaylistItem *item = qobject_cast<PlaylistItem*>(sender());

    int i = d->musicList.indexOf(item);
    emit dataChanged(createIndex(i, 0), createIndex(i, 0));
}

QString PlaylistModel::playlistFilePath() const
{
    QString dirPath = KGlobal::dirs()->saveLocation("data") + KCmdLineArgs::appName();
    QDir().mkdir(dirPath);
    return dirPath + "/playlist";
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
            for (int i=0; i<itemList.count(); i++) {
                QDomNode node = itemList.at(i);
                if (node.isNull()) continue;
                const QString url = node.toElement().attribute("url");
                if (url.isEmpty()) continue;

                const QString name = node.toElement().attribute("name", url);
                const QString artist = node.toElement().attribute("artist", PlaylistItem::defaultArtist);
                const int length = node.toElement().attribute("length", QString::number(PlaylistItem::defaultLength)).toInt();

                PlaylistItem *item = new PlaylistItem(url, name, artist, length, this);
                connect(item, SIGNAL(updated()), SLOT(playlistItemUpdated()));
                d->musicList.append(item);
            }
        }
    }
}

void PlaylistModel::saveToFile(const QString& path)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QDomDocument doc;
        QDomElement playlist = doc.createElement("playlist");

        Q_FOREACH (const PlaylistItem *item, d->musicList) {
            QDomElement element = doc.createElement("item");
            element.setAttribute("url", item->mediaUrl());
            element.setAttribute("name", item->mediaName());
            element.setAttribute("artist", item->mediaArtist());
            element.setAttribute("length", item->mediaLength());
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
