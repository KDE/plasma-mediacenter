/***********************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>          `                     *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2011 Marco Martin <notmart@gmail.com>                               *
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

#include "pmcmetadatamodel.h"
#include "pmcimagecache.h"
#include "pmcimageprovider.h"
#include "metadataupdater.h"
#include "lastfmimagefetcher.h"
#include "media.h"
#include "medialibrary.h"
#include "pmcmedia.h"

#include <KIO/PreviewJob>
#include <KDebug>

#include <QtCore/QTimer>

class PmcMetadataModel::Private
{
public:
    Private()
        : thumbnailerPlugins(new QStringList(KIO::PreviewJob::availablePlugins()))
        , isSearchTermValid(false)
    {
    }
    bool isSearchTermValid;

    //Thumbnail stuff
    const QStringList *thumbnailerPlugins;
    QHash<KUrl, QPersistentModelIndex> filesToPreview;
    QSize thumbnailSize;
    QHash<KUrl, QPersistentModelIndex> previewJobs;
    QTimer previewTimer;
    QTimer updateTimer;
    QTimer metadataFetchTimer;
    QList< QHash<int, QVariant> > metadataValues;
    QList<int> rowsToFetchMetadataFor;
    QStringList mediaUrlWhichFailedThumbnailGeneration;
    QVariant defaultDecoration;
    QString mediaType;
};

PmcMetadataModel::PmcMetadataModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private())
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    d->previewTimer.setSingleShot(true);
    connect(&d->previewTimer, SIGNAL(timeout()), SLOT(delayedPreview()));
    d->updateTimer.setSingleShot(true);
    connect(&d->updateTimer, SIGNAL(timeout()), SLOT(updateModel()));
    d->metadataFetchTimer.setSingleShot(true);
    connect(&d->metadataFetchTimer, SIGNAL(timeout()), SLOT(fetchMetadata()));

    d->thumbnailSize = QSize(512, 512);

    connect(LastFmImageFetcher::instance(), SIGNAL(imageFetched(QPersistentModelIndex,QString)), SLOT(signalUpdate(QPersistentModelIndex,QString)));
}

PmcMetadataModel::~PmcMetadataModel()
{
    delete d;
}

void PmcMetadataModel::updateModel()
{
    emit queryStarted();
}

void PmcMetadataModel::showMediaType(MediaCenter::MediaType mediaType)
{
    switch (mediaType) {
        case MediaCenter::Music:
            d->mediaType ="audio";
            break;
        case MediaCenter::Picture:
            d->mediaType = "image";
            break;
        case MediaCenter::Video:
            d->mediaType = "video";
    }
    QList <QSharedPointer<PmcMedia> > mediaData = MediaLibrary::instance()->getMedia(d->mediaType);
    connect(MediaLibrary::instance(),
            SIGNAL(newMedia(QList<QSharedPointer<PmcMedia> >)),
            SLOT(handleNewMedia(QList<QSharedPointer<PmcMedia> >)));

    handleNewMedia(mediaData);
}

void PmcMetadataModel::showAlbums()
{
    QList <QSharedPointer<PmcAlbum> > mediaData = MediaLibrary::instance()->getAlbums();

    //TODO : Album updates

    handleNewAlbums(mediaData);
}

void PmcMetadataModel::showArtist()
{
    QList <QSharedPointer<PmcArtist> > mediaData = MediaLibrary::instance()->getArtists();
    handleNewArtists(mediaData);
}

void PmcMetadataModel::handleNewMedia(const QList< QSharedPointer< PmcMedia > >& media)
{
    const int existingRowCount = rowCount();

    QList< QHash<int, QVariant> > mediaInfoToInsert;
    foreach (QSharedPointer<PmcMedia> m, media) {
        if (m->type() == d->mediaType) {
            QHash<int, QVariant> mediainfo;
            mediainfo.insert(Qt::DisplayRole, m->title());
            mediainfo.insert(Qt::DecorationRole, m->thumbnail());
            mediainfo.insert(MediaCenter::MediaUrlRole, m->url());
            mediainfo.insert(MediaCenter::MediaTypeRole, m->type());
            mediaInfoToInsert.append(mediainfo);
        }
    }

    beginInsertRows(QModelIndex(), existingRowCount,
                    existingRowCount + mediaInfoToInsert.count());

    d->metadataValues.append(mediaInfoToInsert);
    endInsertRows();
}

void PmcMetadataModel::handleNewAlbums(const QList< QSharedPointer< PmcAlbum > >& mediaData)
{
    const int existingRowCount = rowCount();

    QList< QHash<int, QVariant> > mediaInfoToInsert;
    foreach (QSharedPointer<PmcAlbum> a, mediaData) {
        QHash<int, QVariant> mediainfo;
        mediainfo.insert(Qt::DisplayRole, a->name());
        mediainfo.insert(MediaCenter::MediaTypeRole, "album");
        mediaInfoToInsert.append(mediainfo);
    }

    beginInsertRows(QModelIndex(), existingRowCount,
                    existingRowCount + mediaInfoToInsert.count());

    d->metadataValues.append(mediaInfoToInsert);
    endInsertRows();
}

void PmcMetadataModel::handleNewArtists(const QList< QSharedPointer< PmcArtist > >& mediaData)
{
    const int existingRowCount = rowCount();
    QList< QHash<int, QVariant> > mediaInfoToInsert;
    foreach (QSharedPointer<PmcArtist> a, mediaData) {
        QHash<int, QVariant> mediainfo;
        mediainfo.insert(Qt::DisplayRole, a->name());
        mediainfo.insert(MediaCenter::MediaTypeRole, "artist");
        mediaInfoToInsert.append(mediainfo);
    }

    beginInsertRows(QModelIndex(), existingRowCount,
                    existingRowCount + mediaInfoToInsert.count());

    d->metadataValues.append(mediaInfoToInsert);
    endInsertRows();
}


QVariant PmcMetadataModel::metadataValueForRole(const QModelIndex& index, int role) const
{
    return d->metadataValues.at(index.row()).value(role);
}

QVariant PmcMetadataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();

    const QVariant metadataValue = metadataValueForRole(index, role);
    switch(role) {
    case MediaCenter::ResourceIdRole:
    case MediaCenter::MediaUrlRole:
    case MediaCenter::MediaTypeRole:
    case Qt::DisplayRole:
        return metadataValue;
    case Qt::DecorationRole:
        return decorationForMetadata(metadataValue, index);
    case MediaCenter::MediaThumbnailRole:
        if (metadataValueForRole(index, MediaCenter::MediaTypeRole) == "video") {
            KUrl url = d->metadataValues.value(index.row()).value(MediaCenter::MediaUrlRole).toUrl();
            if (d->mediaUrlWhichFailedThumbnailGeneration.contains(url.prettyUrl()))
                return "image-missing";
            return const_cast<PmcMetadataModel*>(this)->fetchPreview(url, index);
        }
    }

    return QVariant();
}

QVariant PmcMetadataModel::decorationForMetadata(const QVariant &metadataValue, const QModelIndex &index) const
{
    if (metadataValue.isNull() || (metadataValue.type() == QVariant::String && metadataValue.toString().isEmpty())) {
        const QString mediaType = metadataValueForRole(index, MediaCenter::MediaTypeRole).toString();
        if (mediaType == "album") {
            const QString albumName = metadataValueForRole(index, Qt::DisplayRole).toString();
            const QString albumUri = QString("album:%1").arg(albumName);
            if (PmcImageCache::instance()->containsId(albumUri)) {
                return QString("image://%1/%2").arg(PmcImageProvider::identificationString, albumUri);
            } else {
                //FIXME: Album no longer has artist
                //MediaUrlRole is abused for albums to pass on the artist name
                const QString artistNameForAlbum = metadataValueForRole(index, MediaCenter::MediaUrlRole).toString();
                LastFmImageFetcher::instance()->fetchImage("album", QPersistentModelIndex(index), artistNameForAlbum, albumName);
            }
        } else if (mediaType == "artist") {
            const QString artistName = metadataValueForRole(index, Qt::DisplayRole).toString();
            const QString artistUri = QString("artist:%1").arg(artistName);
            if (PmcImageCache::instance()->containsId(artistUri)) {
                return QString("image://%1/%2").arg(PmcImageProvider::identificationString, artistUri);
            } else {
                LastFmImageFetcher::instance()->fetchImage("artist", QPersistentModelIndex(index), artistName);
            }
        }
        if (d->defaultDecoration.isValid()) {
            return d->defaultDecoration;
        }
    }
    return metadataValue;
}

void PmcMetadataModel::fetchMetadata()
{
    d->rowsToFetchMetadataFor.clear();
}


int PmcMetadataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return d->metadataValues.count();
}

QString PmcMetadataModel::fetchPreview(const KUrl &url, const QModelIndex& index)
{
    const QString prettyUrl = url.prettyUrl();
    if (PmcImageCache::instance()->containsId(prettyUrl)) {
        return "image://" + QString(PmcImageProvider::identificationString) + "/" + prettyUrl;
    }

    d->filesToPreview.insert(url, QPersistentModelIndex(index));
    d->previewTimer.start(100);
    return QString();
}

void PmcMetadataModel::delayedPreview()
{
    QHash<KUrl, QPersistentModelIndex>::const_iterator i = d->filesToPreview.constBegin();

    KFileItemList list;

    while (i != d->filesToPreview.constEnd()) {
        KUrl file = i.key();
        QPersistentModelIndex index = i.value();

        if (!d->previewJobs.contains(file) && file.isValid()) {
            list.append(KFileItem(file, QString(), 0));
            d->previewJobs.insert(file, QPersistentModelIndex(index));
        }
        ++i;
    }

    if (list.size() > 0) {
        KIO::PreviewJob* job = KIO::filePreview(list, d->thumbnailSize, d->thumbnailerPlugins);
        connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(showPreview(KFileItem,QPixmap)));
        connect(job, SIGNAL(failed(KFileItem)), SLOT(previewFailed(KFileItem)));
    }

    d->filesToPreview.clear();
}

void PmcMetadataModel::showPreview(const KFileItem &item, const QPixmap &preview)
{
    QPersistentModelIndex index = d->previewJobs.value(item.url());
    d->previewJobs.remove(item.url());

    if (index.isValid()) {
        PmcImageCache::instance()->addImage(item.url().prettyUrl(), preview.toImage());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::previewFailed(const KFileItem &item)
{
    QPersistentModelIndex index = d->previewJobs.value(item.url());
    d->previewJobs.remove(item.url());
    if (index.isValid()) {
        d->mediaUrlWhichFailedThumbnailGeneration.append(item.url().prettyUrl());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::handleUpdaterReset()
{
    resetModel();
}

void PmcMetadataModel::resetModel()
{
    beginResetModel();
    d->metadataValues.clear();
    endResetModel();
}

void PmcMetadataModel::setSearchTerm(const QString& searchTerm)
{
    kDebug() << "NOT IMPLEMENTED!!";
}

void PmcMetadataModel::setDefaultDecoration ( const QVariant& decoration )
{
    d->defaultDecoration = decoration;
}

void PmcMetadataModel::signalUpdate(const QPersistentModelIndex& index, const QString& displayString)
{
    if (!displayString.isEmpty() &&
        metadataValueForRole(index, Qt::DisplayRole).toString() != displayString) {
        return;
    }

    emit dataChanged(index, index);
}

#include "pmcmetadatamodel.moc"
