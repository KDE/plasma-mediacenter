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
#include "lastfmimagefetcher.h"
#include "media.h"
#include "medialibrary.h"
#include "pmcmedia.h"
#include "singletonfactory.h"
#include "pmcalbum.h"
#include "pmcartist.h"
#include "pmccoverartprovider.h"

#include <KIO/PreviewJob>
#include <QDebug>

#include <QtCore/QTimer>

class PmcMetadataModel::Private
{
public:
    Private()
        : thumbnailerPlugins(new QStringList(KIO::PreviewJob::availablePlugins()))
        , isSearchTermValid(false)
    {
        modeForMediaType["audio"] = Music;
        modeForMediaType["image"] = Picture;
        modeForMediaType["video"] = Video;
    }

    //Thumbnail stuff
    const QStringList *thumbnailerPlugins;
    QHash<QUrl, QPersistentModelIndex> filesToPreview;
    QSize thumbnailSize;
    QHash<QUrl, QPersistentModelIndex> previewJobs;
    QTimer previewTimer;
    QTimer updateTimer;
    QTimer metadataFetchTimer;
    QList< QHash<int, QVariant> > metadataValues;
    QList<int> rowsToFetchMetadataFor;
    QStringList mediaUrlWhichFailedThumbnailGeneration;
    //TODO: serve the default decoration using an image provider
    //  probably something like image://icon/foo would return the system icon foo
    QVariant defaultDecoration;
    Mode currentMode;

    bool isSearchTermValid;

    QStringList mediaResourceIds;
    QHash< QString, QSharedPointer<QObject> > mediaByResourceId;

    QHash<QString, Mode> modeForMediaType;
    MediaLibrary *mediaLibrary;
};

PmcMetadataModel::PmcMetadataModel(QObject* parent, MediaLibrary* mediaLibrary):
    QAbstractListModel(parent),
    d(new Private())
{
    d->mediaLibrary = mediaLibrary ? mediaLibrary : SingletonFactory::instanceFor<MediaLibrary>();

    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    d->previewTimer.setSingleShot(true);
    connect(&d->previewTimer, SIGNAL(timeout()), SLOT(delayedPreview()));
    d->updateTimer.setSingleShot(true);
    connect(&d->updateTimer, SIGNAL(timeout()), SLOT(updateModel()));
    d->metadataFetchTimer.setSingleShot(true);
    connect(&d->metadataFetchTimer, SIGNAL(timeout()), SLOT(fetchMetadata()));

    d->thumbnailSize = QSize(512, 512);

    connect(SingletonFactory::instanceFor<LastFmImageFetcher>(),
            SIGNAL(imageFetched(QVariant,QString)),
            SLOT(signalUpdate(QVariant,QString)));
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
            d->currentMode = Music;
            break;
        case MediaCenter::Picture:
            d->currentMode = Picture;
            break;
        case MediaCenter::Video:
            d->currentMode = Video;
    }
    const QString mediaTypeString = d->modeForMediaType.key(d->currentMode);
    QList <QSharedPointer<PmcMedia> > mediaData = d->mediaLibrary->getMedia(mediaTypeString);

    connect(d->mediaLibrary,
            SIGNAL(newMedia(QList<QSharedPointer<PmcMedia> >)),
            SLOT(handleNewMedia(QList<QSharedPointer<PmcMedia> >)));

    handleNewMedia(mediaData);
}

void PmcMetadataModel::showAlbums()
{
    QList <QSharedPointer<PmcAlbum> > mediaData = d->mediaLibrary->getAlbums();
    d->currentMode = Album;

    connect(d->mediaLibrary,
            SIGNAL(newAlbums(QList<QSharedPointer<PmcAlbum> >)),
            SLOT(handleNewAlbums(QList<QSharedPointer<PmcAlbum> >)));
    handleNewAlbums(mediaData);
}

void PmcMetadataModel::showArtist()
{
    QList <QSharedPointer<PmcArtist> > mediaData = d->mediaLibrary->getArtists();
    d->currentMode = Artist;

    connect(d->mediaLibrary,
            SIGNAL(newArtists(QList<QSharedPointer<PmcArtist> >)),
            SLOT(handleNewArtists(QList<QSharedPointer<PmcArtist> >)));
    handleNewArtists(mediaData);
}

void PmcMetadataModel::handleNewMedia(const QList< QSharedPointer< PmcMedia > >& media)
{
    const int existingRowCount = rowCount();
    QStringList resourceIdsToBeInserted;

    Q_FOREACH (const QSharedPointer<PmcMedia> &m, media) {
        if (d->modeForMediaType.value(m->type()) == d->currentMode) {
            d->mediaByResourceId.insert(m->sha(), QSharedPointer<QObject>(m));
            resourceIdsToBeInserted.append(m->sha());
            connect(m.data(), SIGNAL(updated()), SLOT(mediaUpdated()));
        }
    }

    if (resourceIdsToBeInserted.size() > 0) {
        beginInsertRows(QModelIndex(), existingRowCount,
                        existingRowCount + resourceIdsToBeInserted.size() - 1);
        d->mediaResourceIds.append(resourceIdsToBeInserted);
        Q_ASSERT(d->mediaByResourceId.keys().size() == d->mediaResourceIds.size());
        endInsertRows();
    }
}

void PmcMetadataModel::handleNewAlbums(const QList< QSharedPointer< PmcAlbum > >& mediaData)
{
    handleNewAlbumsOrArtists<PmcAlbum>(mediaData);
}

void PmcMetadataModel::handleNewArtists(const QList< QSharedPointer< PmcArtist > >& mediaData)
{
    handleNewAlbumsOrArtists<PmcArtist>(mediaData);
}

template <class T>
void PmcMetadataModel::handleNewAlbumsOrArtists(const QList< QSharedPointer< T > > &mediaData)
{
    const int existingRowCount = rowCount();
    QStringList resourceIdsToBeInserted;

    Q_FOREACH (const QSharedPointer<T> &a, mediaData) {
        if (d->mediaByResourceId.contains(a->name())) {
            qWarning() << "ALREADY HAS " << a->name();
            continue;
        }
        d->mediaByResourceId.insert(a->name(), QSharedPointer<QObject>(a));
        resourceIdsToBeInserted.append(a->name());
    }

    if (resourceIdsToBeInserted.size() > 0) {
        beginInsertRows(QModelIndex(), existingRowCount,
                        existingRowCount + resourceIdsToBeInserted.size() - 1);
        d->mediaResourceIds.append(resourceIdsToBeInserted);

        Q_ASSERT(d->mediaByResourceId.keys().size() == d->mediaResourceIds.size());
        endInsertRows();
    }
}

QVariant PmcMetadataModel::metadataValueForRole(const QModelIndex& index, int role) const
{
    //FIXME: If the logic is correct elsewhere, this check should not be required
    return index.isValid() && index.row() > 0 && index.row() < d->metadataValues.size() ?
        d->metadataValues.at(index.row()).value(role) : QVariant();
}

QVariant PmcMetadataModel::data(const QModelIndex& index, int role) const
{
    const int row = index.row();
    if (!index.isValid() || row >= rowCount())
        return QVariant();

    switch (d->currentMode) {
        case Music:
        case Picture:
        case Video:
            return dataForMedia(index, role);
        case Album:
            return dataForAlbum(row, role);
        case Artist:
            return dataForArtist(row, role);
    }

    return QVariant();
}

QVariant PmcMetadataModel::dataForMedia(const QModelIndex &index, int role) const
{
    const int row = index.row();

    const QString resourceId = d->mediaResourceIds.at(row);
    const QSharedPointer<QObject> mediaObject = d->mediaByResourceId.value(d->mediaResourceIds.at(row));
    const QSharedPointer<PmcMedia> media = qSharedPointerObjectCast<PmcMedia>(mediaObject);

    switch (role) {
    case MediaCenter::ResourceIdRole:
        return media->sha();
    case MediaCenter::MediaUrlRole:
        return media->url();
    case MediaCenter::MediaTypeRole:
        return media->type();
    case Qt::DisplayRole:
        return media->title();
    case MediaCenter::AlbumRole:
        return media->album();
    case MediaCenter::ArtistRole:
        return media->artist();
    case MediaCenter::DurationRole:
        return media->duration();
    case MediaCenter::GenreRole:
        return media->genre();
    case Qt::DecorationRole:
        if (media->type() == "video") {
            const QUrl url(media->url());
            if (d->mediaUrlWhichFailedThumbnailGeneration.contains(url.url()))
                return "image-missing";
            return const_cast<PmcMetadataModel*>(this)->fetchPreview(url, index);
        } else if (media->type() == "audio") {
            return getAlbumArt(media->album(), media->artist(), resourceId);
        } else {
            return media->thumbnail();
        }
    case MediaCenter::CreatedAtRole:
        return media->createdAt();
    }

    return QVariant();
}

QVariant PmcMetadataModel::dataForAlbum(int row, int role) const
{
    const QString resourceId = d->mediaResourceIds.at(row);
    const QSharedPointer<QObject> mediaObject = d->mediaByResourceId.value(resourceId);
    const QSharedPointer<PmcAlbum> album = qSharedPointerObjectCast<PmcAlbum>(mediaObject);

    switch (role) {
    case Qt::DisplayRole:
        return album->name();
    case Qt::DecorationRole:
        return getAlbumArt(album->name(), album->albumArtist(), resourceId);
    case MediaCenter::IsExpandableRole:
        return true;
    }

    return QVariant();
}

QVariant PmcMetadataModel::dataForArtist(int row, int role) const
{
    const QString resourceId = d->mediaResourceIds.at(row);
    const QSharedPointer<QObject> mediaObject = d->mediaByResourceId.value(resourceId);
    const QSharedPointer<PmcArtist> artist = qSharedPointerObjectCast<PmcArtist>(mediaObject);

    switch (role) {
    case Qt::DisplayRole:
        return artist->name();
    case Qt::DecorationRole:
        return getArtistImage(artist->name(), resourceId);
    case MediaCenter::IsExpandableRole:
        return true;
    }

    return QVariant();
}

QVariant PmcMetadataModel::getAlbumArt(const QString& albumName, const QString& albumArtist, const QString &resourceId) const
{
    PmcImageCache *imageCache = SingletonFactory::instanceFor<PmcImageCache>();

    if (imageCache->containsAlbumCover(albumName)) {
        return PmcCoverArtProvider::qmlImageUriForAlbumCover(albumName);
    } else {
        SingletonFactory::instanceFor<LastFmImageFetcher>()->fetchImage("album", resourceId, albumArtist, albumName);
    }

    return d->defaultDecoration;
}

QVariant PmcMetadataModel::getArtistImage(const QString& artistName, const QString& resourceId) const
{
    PmcImageCache *imageCache = SingletonFactory::instanceFor<PmcImageCache>();

    if (imageCache->containsArtistCover(artistName)) {
        return PmcCoverArtProvider::qmlImageUriForArtistCover(artistName);
    } else {
        SingletonFactory::instanceFor<LastFmImageFetcher>()->fetchImage("artist", resourceId, artistName);
    }

    return d->defaultDecoration;
}

void PmcMetadataModel::fetchMetadata()
{
    d->rowsToFetchMetadataFor.clear();
}


int PmcMetadataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return d->mediaResourceIds.size();
}

QString PmcMetadataModel::fetchPreview(const QUrl &url, const QModelIndex& index)
{
    PmcImageCache *imageCache = SingletonFactory::instanceFor<PmcImageCache>();
    const QString prettyUrl = url.url();

    if (imageCache->containsMediaFileCover(prettyUrl)) {
        return PmcCoverArtProvider::qmlImageUriForMediaFileCover(prettyUrl);
    }

    d->filesToPreview.insert(url, QPersistentModelIndex(index));
    d->previewTimer.start(100);
    return d->defaultDecoration.toString();
}

void PmcMetadataModel::delayedPreview()
{
    QHash<QUrl, QPersistentModelIndex>::const_iterator i = d->filesToPreview.constBegin();

    KFileItemList list;

    while (i != d->filesToPreview.constEnd()) {
        QUrl file = i.key();
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
        SingletonFactory::instanceFor<PmcImageCache>()->addMediaFileCover(item.url().url(), preview.toImage());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::previewFailed(const KFileItem &item)
{
    QPersistentModelIndex index = d->previewJobs.value(item.url());
    d->previewJobs.remove(item.url());
    if (index.isValid()) {
        d->mediaUrlWhichFailedThumbnailGeneration.append(item.url().url());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::setDefaultDecoration(const QVariant& decoration)
{
    d->defaultDecoration = decoration;
}

void PmcMetadataModel::signalUpdate(const QVariant& resourceId, const QString& displayString)
{
    if (displayString.isEmpty() || !d->mediaByResourceId.contains(resourceId.toString())) {
        return;
    }

    const int rowForResource = d->mediaResourceIds.indexOf(resourceId.toString());
    const QModelIndex changedIndex = index(rowForResource);
    emit dataChanged(changedIndex, changedIndex);
}

void PmcMetadataModel::mediaUpdated()
{
    PmcMedia *media = static_cast<PmcMedia*>(sender());
    const QString resourceId = media->sha();

    const int mediaIndex = d->mediaResourceIds.indexOf(resourceId);
    const QModelIndex changedIndex = index(mediaIndex);
    emit dataChanged(changedIndex, changedIndex);
}
