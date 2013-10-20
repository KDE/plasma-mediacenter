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

#ifndef MULTIPLEPLAYLISTMODEL_H
#define MULTIPLEPLAYLISTMODEL_H

#include <QtCore/QAbstractListModel>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "mediacenter_export.h"
#include "mediacenter.h"
#include "playlistmodel.h"

class MEDIACENTER_EXPORT MultiplePlaylistModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MultiplePlaylistModel(QObject* parent = 0);
    ~MultiplePlaylistModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Q_INVOKABLE  void createNewPlaylist(const QString &name);
    Q_INVOKABLE void setPlaylistModelAddress (QObject *model);
    Q_INVOKABLE void switchToPlaylist (QString name);
    
private:
    QStringList m_multiplePlaylistList;
    PlaylistModel *m_playlistModel;
    
};

#endif  // MULTIPLEPLAYLISTMODEL_H