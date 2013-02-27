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
#include <KDE/KStandardDirs>
#include <KDE/KCmdLineArgs>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QDateTime>
#include <KConfigGroup>
#include <kconfig.h>

PlaylistModel::PlaylistModel(QObject* parent):
    QAbstractListModel(parent)
{
    KConfigGroup cfgGroup = KGlobal::config()->group("General");
    setRandom(cfgGroup.readEntry("randomplaylist",false));
    KStandardDirs dir;
    QString dirPath = dir.saveLocation("data") + KCmdLineArgs::appName();
    QDir().mkdir(dirPath);
    m_filePath = dirPath + "/playlist";

    if (QFile::exists(m_filePath)) {
        QFile file(m_filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                m_musicList.append(PlaylistItem::fromString(line));
            }
        }
        file.close();
    }
    m_currentIndex = -1;
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    qsrand(QDateTime::currentMSecsSinceEpoch());
}

PlaylistModel::~PlaylistModel()
{
    KConfigGroup cfgGroup = KGlobal::config()->group("General");
    cfgGroup.writeEntry("randomplaylist",m_random);
    cfgGroup.sync();
    QFile file(m_filePath);
    if (file.open(QIODevice::WriteOnly)) {
         QTextStream out(&file);
         foreach (PlaylistItem item, m_musicList) {
             out << item.intoString() << "\n";
         }
    }
    file.close();
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
    const int n = rowCount();
    beginInsertRows(QModelIndex(), n, n);

    PlaylistItem item;
    item.setMediaName(name);
    item.setMediaUrl(url);
    m_musicList.append(item);

    endInsertRows();
}

void PlaylistModel::removeFromPlaylist(const int& index)
{
    beginResetModel();
    m_musicList.removeAt(index);
    if (index <= m_currentIndex) {
        m_currentIndex -= 1;
    }
    endResetModel();
}

QString PlaylistModel::getNextUrl()
{
    if (random()) {
        setCurrentIndex(qrand() % m_musicList.size());
    } else if (m_currentIndex == m_musicList.count() - 1) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(m_currentIndex + 1);
    }
    return m_musicList.at(m_currentIndex).mediaUrl();
}

QString PlaylistModel::getPreviousUrl()
{
    if (random()) {
        setCurrentIndex(qrand() % m_musicList.size());
    } else if (m_currentIndex == 0) {
        setCurrentIndex(m_musicList.count() - 1);
    } else {
        setCurrentIndex(m_currentIndex - 1);
    }
    return m_musicList.at(m_currentIndex).mediaUrl();
}

QString PlaylistModel::getUrlofFirstIndex()
{
    return m_musicList.isEmpty() ? QString() : m_musicList.at(0).mediaUrl();
}

void PlaylistModel::clearPlaylist()
{
    beginResetModel();

    m_musicList.clear();
    m_currentIndex = -1;

    endResetModel();
}

int PlaylistModel::currentIndex() const
{
    return m_currentIndex;
}

void PlaylistModel::setCurrentIndex(int index)
{
    m_currentIndex = index;
    emit currentIndexChanged();
}

QString PlaylistItem::intoString()
{
    return  (mediaName() + "/" + mediaUrl());
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
    return m_random;
}

void PlaylistModel::setRandom ( bool random )
{
    m_random = random;
    emit randomChanged();
}
