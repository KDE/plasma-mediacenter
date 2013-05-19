/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                               *
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

#include "playlistmodel.h"
#include "playlistitem.h"

#include <KDebug>
#include <KDE/KStandardDirs>
#include <KDE/KCmdLineArgs>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <KConfigGroup>
#include <kconfig.h>

class PlaylistModel::Private
{
public:
    QList<PlaylistItem*> musicList;
    int currentIndex;
    QFile file;
    QString filePath;
    bool random;
};

PlaylistModel::PlaylistModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private)
{
    KConfigGroup cfgGroup = KGlobal::config()->group("General");
    setRandom(cfgGroup.readEntry("randomplaylist",false));
    QString dirPath = KGlobal::dirs()->saveLocation("data") + KCmdLineArgs::appName();
    QDir().mkdir(dirPath);
    d->filePath = dirPath + "/playlist";

    if (QFile::exists(d->filePath)) {
        QFile file(d->filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                PlaylistItem *item = new PlaylistItem(line, this);
                connect(item, SIGNAL(updated()), SLOT(playlistItemUpdated()));
                d->musicList.append(item);
            }
        }
        file.close();
    }
    d->currentIndex = -1;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    QHash<int, QByteArray> newRoles(roleNames());
    newRoles[MediaLengthRole] = "mediaLength";
    newRoles[MediaArtistRole] = "mediaArtist";
    setRoleNames(newRoles);

    qsrand(QDateTime::currentMSecsSinceEpoch());
}

PlaylistModel::~PlaylistModel()
{
    KConfigGroup cfgGroup = KGlobal::config()->group("General");
    cfgGroup.writeEntry("randomplaylist",d->random);
    cfgGroup.sync();
    QFile file(d->filePath);
    if (file.open(QIODevice::WriteOnly)) {
         QTextStream out(&file);
         Q_FOREACH (const PlaylistItem *item, d->musicList) {
             out << item->mediaUrl() << "\n";
         }
    }
    file.close();
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

QString PlaylistModel::getNextUrl()
{
    if (random()) {
        setCurrentIndex(qrand() % d->musicList.size());
    } else if (d->currentIndex == d->musicList.count() - 1) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(d->currentIndex + 1);
    }
    return d->musicList.at(d->currentIndex)->mediaUrl();
}

QString PlaylistModel::getPreviousUrl()
{
    if (random()) {
        setCurrentIndex(qrand() % d->musicList.size());
    } else if (d->currentIndex == 0) {
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

bool PlaylistModel::random() const
{
    return d->random;
}

void PlaylistModel::setRandom ( bool random )
{
    d->random = random;
    emit randomChanged();
}

void PlaylistModel::playlistItemUpdated()
{
    PlaylistItem *item = qobject_cast<PlaylistItem*>(sender());

    int i = d->musicList.indexOf(item);
    emit dataChanged(createIndex(i, 0), createIndex(i, 0)); 
}
