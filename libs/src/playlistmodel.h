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

#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QtCore/QAbstractItemModel>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QFile>

#include "kmediacollection_export.h"
#include "kmediacollection.h"

class KCmdLineArgs;
class KMEDIACOLLECTION_EXPORT PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(QString currentUrl READ currentUrl NOTIFY currentUrlChanged)

public:
    enum Roles {
        MediaArtistRole = KMediaCollection::AdditionalRoles + 1,
        MediaLengthRole,
        OriginalIndexRole
    };

    explicit PlaylistModel(QObject* parent = 0);
    ~PlaylistModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    Q_INVOKABLE  void addToPlaylist(const QString &url);
    Q_INVOKABLE  void addAllToPlaylist(const QStringList &list);
    Q_INVOKABLE  void playNext();
    Q_INVOKABLE  void playPrevious();
    Q_INVOKABLE void clearPlaylist();
    Q_INVOKABLE void removeFromPlaylist(const int &index);
    Q_INVOKABLE QString getUrlofFirstIndex();
    Q_INVOKABLE void moveItem(int originalIndex, int newIndex);
    int currentIndex() const;
    void setCurrentIndex(int index);
    bool random() const;
    Q_INVOKABLE void shuffle();
    bool removeCurrentPlaylist (const QString& playlistToSwitchToAfterDeletion);
    bool processCommandLineArgs(const QStringList *urls);
    void play(int index);

    QString playlistName() const;
    void setPlaylistName(const QString &name);
    QString getPlaylistPath() const;

    QString currentUrl() const;

Q_SIGNALS:
    void currentIndexChanged();
    void randomChanged();
    void currentUrlChanged();
    void playlistNameChanged();

public Q_SLOTS:
    void savePlaylist();
    void resetCurrentIndex();
    void switchToDefaultPlaylist();

private Q_SLOTS:
    void playlistItemUpdated();

private:
    class Private;
    Private * const d;

    void loadFromFile(const QString &path);
    void saveToFile(const QString &path) const;
    QString playlistFilePath() const;
    void clearPlaylistWithoutModelReset();
    int addToPlaylist(const QStringList &urls);
};

#endif // PLAYLISTMODEL_H
