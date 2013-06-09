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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "mediacenter_export.h"
#include "mediacenter.h"

class MEDIACENTER_EXPORT PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)

public:
    enum Roles {
        MediaArtistRole = MediaCenter::AdditionalRoles + 1,
        MediaLengthRole
    };

    explicit PlaylistModel(QObject* parent = 0);
    ~PlaylistModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Q_INVOKABLE  void addToPlaylist(const QString &url);
    Q_INVOKABLE QString getNextUrl();
    Q_INVOKABLE QString getPreviousUrl();
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE void removeFromPlaylist(const int &index);
    Q_INVOKABLE QString getUrlofFirstIndex();
    Q_INVOKABLE void moveItem(int firstIndex,int secondIndex);
    int currentIndex() const;
    void setCurrentIndex(int index);
    bool random() const;
    Q_INVOKABLE void shuffle();

Q_SIGNALS:
    void currentIndexChanged();
    void randomChanged();

private Q_SLOTS:
    void playlistItemUpdated();

private:
    class Private;
    Private * const d;
};

#endif // PLAYLISTMODEL_H

