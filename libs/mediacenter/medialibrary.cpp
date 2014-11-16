/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#include "medialibrary.h"

#include <QVariant>
#include <QTimer>
#include <QApplication>
#include <QPointer>
#include <QDir>

#include <QDebug>

#include "media.h"

#include "mediacenter.h"
#include "medialibrarywrappercache.h"
#include "mediavalidator.h"

#include "pmcmedia.h"
#include "pmcalbum.h"
#include "pmcartist.h"

#include "artist.h"
#include "album.h"
#include "itemcache.h"

namespace {
    int DELAY_BEFORE_EMITTING_NEW_ITEMS = 1000;
}

class MediaLibrary::Private
{
public:
    QList< QPair <QString, QHash<int, QVariant> > > updateRequests;
    QMutex updateRequestsMutex;

    QList<Media> mediaToPersist;
    QMutex mediaToPersistMutex;

    QMutex mediaMutex;
    QHash <QString, QSharedPointer<Media> > mediaBySha;

    QList< QSharedPointer<PmcMedia> > newMediaList;
    QList< QSharedPointer<PmcAlbum> > newAlbumList;
    QList< QSharedPointer<PmcArtist> > newArtistList;
    QPointer<QTimer> newMediaTimer;

    QMutex albumListMutex;
    QList< QSharedPointer<Album> > albumList;

    QMutex artistListMutex;
    QList< QSharedPointer<Artist> > artistList;

    MediaLibraryWrapperCache *wrapperCache;
    MediaValidator *mediaValidator;

    QMutex pmcMediaByUrlMutex;
    QHash< QString, QSharedPointer<PmcMedia> > pmcMediaByUrl;

    ItemCache itemCache;
};

MediaLibrary::MediaLibrary(MediaValidator* mediaValidator, QObject* parent)
    : QThread(parent)
    , d(new Private())
{
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcMedia> >");
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcAlbum> >");
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcArtist> >");

    d->mediaValidator = mediaValidator;

    moveToThread(this);
}

void MediaLibrary::init()
{
    d->wrapperCache = new MediaLibraryWrapperCache(this);
    if (!d->mediaValidator) {
        d->mediaValidator = new MediaValidator(this);
    }

    d->newMediaTimer = new QTimer(this);
    d->newMediaTimer->setInterval(DELAY_BEFORE_EMITTING_NEW_ITEMS);
    d->newMediaTimer->setSingleShot(true);
    connect(d->newMediaTimer.data(), SIGNAL(timeout()), SLOT(emitNewMediaWithMediaList()));

    emit initialized();
}

MediaLibrary::~MediaLibrary()
{
    qDebug() << "Waiting for Media Library to finish...";
    quit();
    wait();
    delete d;
}

void MediaLibrary::run()
{
    init();
    exec();
}

void MediaLibrary::processRemainingRequests()
{
    Q_ASSERT(thread() == this);

    if (!areThereUpdateRequests()) {
        return;
    }

    while (areThereUpdateRequests()) {
        processNextRequest();
    }
}

void MediaLibrary::processNextRequest()
{
    QPair<QString, QHash<int, QVariant> > request = takeRequest();

//     qDebug() << "Process " << request.first;
    const QString mediaSha = Media::calculateSha(request.first);
    if (mediaExists(mediaSha)) {
        QSharedPointer<Media> media = mediaForSha(mediaSha);

        bool wasUpdated = false;
        Q_FOREACH(int role, request.second.keys()) {
            if (role == MediaCenter::AlbumRole) {
                wasUpdated = extractAndSaveAlbumInfo(request, media) || wasUpdated;
            } else if (role == MediaCenter::ArtistRole) {
                wasUpdated = extractAndSaveArtistInfo(request, media) || wasUpdated;
            } else {
                wasUpdated = media->setValueForRole(role, request.second.value(role)) || wasUpdated;
            }
        }

        if (wasUpdated) {
            emitNewAlbumOrArtistIfNeeded(media);
//             qDebug() << "Updated " << media->url();
        }
    } else {
        if (d->mediaValidator->fileWithUrlExists(request.first)) {
            QSharedPointer<Media> media(new Media(request.first));
            Q_FOREACH(int role, request.second.keys()) {
                if (role == MediaCenter::AlbumRole) {
                    extractAndSaveAlbumInfo(request, media);
                } else if(role == MediaCenter::ArtistRole) {
                    extractAndSaveArtistInfo(request, media);
                } else {
                    media->setValueForRole(role, request.second.value(role));
                }
            }
            addMedia(media);
//             qDebug() << "Saved " << sha;
        }
    }
}

void MediaLibrary::emitNewAlbumOrArtistIfNeeded(QSharedPointer< Media > media)
{
    addArtist(media->artist());
    addAlbum(media->album());
}

bool MediaLibrary::extractAndSaveArtistInfo(
    const QPair< QString, QHash< int, QVariant > >& request,
    QSharedPointer< Media >& media)
{
    QString artistName = request.second.value(MediaCenter::ArtistRole).toString();

    if (artistName.isEmpty()) {
        artistName = "Unknown Artist";
    }
    if (!media->artist().isNull() && media->artist()->name() == artistName) {
        return false;
    }

    QSharedPointer<Artist> artist = d->itemCache.getArtistByName(artistName);

    media->setArtistAndUpdateRelations(media, artist);
    return true;
}

bool MediaLibrary::extractAndSaveAlbumInfo(
    const QPair<QString, QHash<int, QVariant> > &request,
    QSharedPointer<Media> &media)
{
    QString albumName = request.second.value(MediaCenter::AlbumRole).toString();
    QString artistName = request.second.value(MediaCenter::ArtistRole).toString();

    if (albumName.isEmpty()){
        albumName = "Unknown Album";
    }
    if (artistName.isEmpty()) {
        if(!media->artist().isNull()) {
            artistName = media->artist()->name();
        } else {
            artistName = "Unknown Artist";
        }
    }
    if (!media->album().isNull() && media->album()->name() == albumName
        && media->album()->albumArtist() == artistName) {
        return false;
    }

    QSharedPointer<Album> album = d->itemCache.getAlbumByName(albumName, artistName);

    media->setAlbumAndUpdateRelations(media, album);
    return true;
}

bool MediaLibrary::mediaExists(const QString& sha) const
{
    return d->mediaBySha.contains(sha);
}

QSharedPointer< Media > MediaLibrary::mediaForSha(const QString& sha) const
{
    return d->mediaBySha.value(sha);
}

QPair< QString, QHash< int, QVariant > > MediaLibrary::takeRequest()
{
    QMutexLocker locker(&d->updateRequestsMutex);

    return d->updateRequests.takeFirst();
}

bool MediaLibrary::areThereUpdateRequests()
{
    QMutexLocker locker(&d->updateRequestsMutex);

    return !d->updateRequests.isEmpty();
}

void MediaLibrary::updateMedia(const QHash< int, QVariant >& data)
{
    updateMedia(data.value(MediaCenter::MediaUrlRole).toString(), data);
}

void MediaLibrary::updateMedia(const QString& url, const QHash< int, QVariant >& data)
{
    QMutexLocker locker(&d->updateRequestsMutex);

    d->updateRequests.append(QPair<QString, QHash< int, QVariant> >(url, data));

    QTimer::singleShot(0, this, SLOT(processRemainingRequests()));
}

void MediaLibrary::addMedia(const QSharedPointer< Media >& m)
{
    QMutexLocker l(&d->mediaMutex);

    d->mediaBySha.insert(m->sha(), m);
    QSharedPointer<PmcMedia> pmcMedia = mediaForUrl(m->url());
    pmcMedia->setMedia(m);

    d->newMediaList.append(pmcMedia);

    emitNewAlbumOrArtistIfNeeded(m);
    emitNewMedia();
}

void MediaLibrary::addAlbum(const QSharedPointer< Album >& album)
{
    addAlbumOrArtist<Album, PmcAlbum>(album, d->albumListMutex, d->albumList,
                                      d->newAlbumList);
}

void MediaLibrary::addArtist(const QSharedPointer< Artist >& artist)
{
    addAlbumOrArtist<Artist, PmcArtist>(artist, d->artistListMutex,
                                        d->artistList, d->newArtistList);
}

template <class X, class Y>
void MediaLibrary::addAlbumOrArtist(const QSharedPointer< X >& value,
                                    QMutex &mutex,
                                    QList< QSharedPointer< X > >& valueList,
                                    QList< QSharedPointer< Y > >& updatesList)
{
    if (value.isNull()) return;
    QMutexLocker l(&mutex);

    if (!valueList.contains(value)) {
        QSharedPointer<Y> pa(new Y(value));
        valueList.append(value);

        updatesList.append(pa);
        emitNewMedia();
    }
}

QList< QSharedPointer<PmcMedia> > MediaLibrary::getMedia(const QString& type) const
{
    QMutexLocker l(&d->mediaMutex);
    return d->wrapperCache->getMedia(type);
}

void MediaLibrary::emitNewMedia()
{
    if (!d->newMediaTimer->isActive()) {
        d->newMediaTimer->start();
    }
}

#define PMC_ML_EMIT_IF_NEEDED(mutex, list, signal) \
    { \
        QMutexLocker l(&mutex); \
        if (!list.isEmpty()) { \
            qDebug() << "Emitting " << SIGNAL(signal); \
            emit signal(list); \
            list.clear(); \
        } \
    } \

void MediaLibrary::emitNewMediaWithMediaList()
{
    PMC_ML_EMIT_IF_NEEDED(d->mediaMutex, d->newMediaList, newMedia)
    PMC_ML_EMIT_IF_NEEDED(d->albumListMutex, d->newAlbumList, newAlbums)
    PMC_ML_EMIT_IF_NEEDED(d->artistListMutex, d->newArtistList, newArtists)
}

QList< QSharedPointer< PmcAlbum > > MediaLibrary::getAlbums() const
{
    QMutexLocker l(&d->albumListMutex);
    return d->wrapperCache->getAlbums();
}

QList< QSharedPointer< PmcArtist > > MediaLibrary::getArtists() const
{
    QMutexLocker l(&d->artistListMutex);
    return d->wrapperCache->getArtists();
}

QSharedPointer< PmcMedia > MediaLibrary::mediaForUrl(const QString& url) const
{
    QMutexLocker l(&d->pmcMediaByUrlMutex);

    if (d->pmcMediaByUrl.contains(url)) {
        return d->pmcMediaByUrl.value(url);
    }

    QSharedPointer<PmcMedia> pmcMedia(new PmcMedia(url));
    d->pmcMediaByUrl.insert(url, pmcMedia);

    QSharedPointer<Media> media = mediaForSha(Media::calculateSha(url));
    pmcMedia->setMedia(media);

    return pmcMedia;
}
