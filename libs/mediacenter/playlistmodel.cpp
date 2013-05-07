/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmai.com>                               *
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
    QList<PlaylistItem> musicList;
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
                d->musicList.append(PlaylistItem::fromString(line));
            }
        }
        file.close();
    }
    d->currentIndex = -1;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

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
         foreach (const PlaylistItem &item, d->musicList) {
             out << item.intoString() << "\n";
         }
    }
    file.close();
}

QVariant PlaylistModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        return d->musicList.at(index.row()).mediaName();
    }

    if (role == MediaCenter::MediaUrlRole) {
        return d->musicList.at(index.row()).mediaUrl();
    }

    return QVariant();
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    return d->musicList.count();
}

void PlaylistModel::addToPlaylist(const QString& url, const QString& name)
{
    const int n = rowCount();
    beginInsertRows(QModelIndex(), n, n);

    PlaylistItem item;
    item.setMediaName(name);
    item.setMediaUrl(url);
    d->musicList.append(item);

    endInsertRows();
}

void PlaylistModel::removeFromPlaylist(const int& index)
{
    beginResetModel();
    d->musicList.removeAt(index);
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
    return d->musicList.at(d->currentIndex).mediaUrl();
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
    return d->musicList.at(d->currentIndex).mediaUrl();
}

QString PlaylistModel::getUrlofFirstIndex()
{
    return d->musicList.isEmpty() ? QString() : d->musicList.at(0).mediaUrl();
}

void PlaylistModel::clearPlaylist()
{
    beginResetModel();

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

QString PlaylistItem::intoString() const
{
    return  (mediaName() + '/' + mediaUrl());
}

PlaylistItem PlaylistItem::fromString(QString text)
{
    int pos = text.indexOf('/');
    PlaylistItem item;
    item.setMediaUrl(text.right(text.length() - pos - 1));
    item.setMediaName(text.left(pos));
    return item;
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

bool PlaylistModel::random() const
{
    return d->random;
}

void PlaylistModel::setRandom ( bool random )
{
    d->random = random;
    emit randomChanged();
}
