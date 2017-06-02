/***********************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                                *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef PMCMETADATAMODEL_H
#define PMCMETADATAMODEL_H

#include "mediacenter_export.h"
#include "mediacenter.h"

#include <QtCore/QAbstractItemModel>
#include <QSharedPointer>
#include <QList>

class PmcArtist;
class PmcAlbum;
class PmcMedia;
class MediaLibrary;
class QPixmap;
class KFileItem;

class MEDIACENTER_EXPORT PmcMetadataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PmcMetadataModel(QObject* parent = 0, MediaLibrary* mediaLibrary = 0);
    virtual ~PmcMetadataModel();
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void showMediaType(MediaCenter::MediaType mediaType);
    void setDefaultDecoration(const QString &decoration);

public Q_SLOTS:
    void showAlbums();
    void showArtist();

signals:
    void queryStarted();
    void queryFinished();

protected Q_SLOTS:
    void handleNewMedia(const QList<QSharedPointer<PmcMedia> > &media);
    void handleNewAlbums(const QList< QSharedPointer< PmcAlbum > > &mediaData);
    void handleNewArtists(const QList< QSharedPointer< PmcArtist > > &mediaData);
    void updateModel();
    void delayedPreview();

protected:
    QString fetchPreview(const QUrl& url, const QModelIndex& index);

private Q_SLOTS:
    void showPreview(const KFileItem &item, const QPixmap &preview);
    void previewFailed(const KFileItem &item);
    void fetchMetadata();
    void signalUpdate(const QVariant& resourceId, const QString& displayString = QString());
    void mediaUpdated();
    void removeMediaRef(QSharedPointer<PmcMedia> media);

private:
    enum Mode { Music, Picture, Video, Album, Artist };

    class Private;
    Private * const d;

    QVariant metadataValueForRole(const QModelIndex& index, int role) const;
    QVariant dataForMedia(const QModelIndex& row, int role) const;
    QVariant dataForArtist(int row, int role) const;
    QVariant dataForAlbum(int row, int role) const;

    template <class T> void handleNewAlbumsOrArtists(const QList< QSharedPointer< T > > &mediaData);
    template <class T> void removeAlbumOrArtist(const T* albumOrArtist);
    QVariant getAlbumArt(const QString& albumName, const QString& albumArtist, const QString& resourceId) const;
    QVariant getArtistImage(const QString& artistName, const QString& resourceId) const;
    template <class T> void albumOrArtistUpdated(const T* albumOrArtist);

};

#endif // PMCMETADATAMODEL_H
