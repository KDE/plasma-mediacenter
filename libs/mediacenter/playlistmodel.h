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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QString>
#include <QList>
#include "mediacenter_export.h"
#include "mediacenter.h"

class PlaylistItem
{

public:
    void setMediaName(const QString name);
    void setMediaUrl(const QString url);
    QString mediaName() const;
    QString mediaUrl() const;

private:
    QString m_mediaName;
    QString m_mediaUrl;
};

class MEDIACENTER_EXPORT PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    explicit PlaylistModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Q_INVOKABLE  void addToPlaylist(const QString &url, const QString &name);
    Q_INVOKABLE QString getNextUrl();
    Q_INVOKABLE QString getPreviousUrl();
    Q_INVOKABLE void clearPlaylist();
    int currentIndex();
    void setCurrentIndex(int index);

signals:
    void currentIndexChanged();

private:
    QList<PlaylistItem> m_musicList;
    int m_currentIndex;
};

#endif // PLAYLISTMODEL_H
