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

#include "medialibrarywrappercache.h"

#include "pmcmedia.h"
#include "pmcalbum.h"
#include "pmcartist.h"

#include "medialibrary.h"

class MediaLibraryWrapperCache::Private
{
public:
    QList<QSharedPointer<PmcAlbum> > pmcAlbums;
    QList<QSharedPointer<PmcArtist> > pmcArtists;
    QHash <QString, QList<QSharedPointer<PmcMedia> > > pmcMediaByType;
};

MediaLibraryWrapperCache::MediaLibraryWrapperCache(MediaLibrary* parent)
    : QObject(parent)
    , d(new Private())
{
    connect(parent, &MediaLibrary::newMedia, this, &MediaLibraryWrapperCache::saveNewMedia);
    connect(parent, &MediaLibrary::newAlbums, this, &MediaLibraryWrapperCache::saveNewAlbums);
    connect(parent, &MediaLibrary::newArtists, this, &MediaLibraryWrapperCache::saveNewArtists);
    connect(parent, &MediaLibrary::mediaRemoved, this, &MediaLibraryWrapperCache::removeMediaRef);
}


MediaLibraryWrapperCache::~MediaLibraryWrapperCache()
{
    delete d;
}

void MediaLibraryWrapperCache::saveNewMedia(const QList< QSharedPointer< PmcMedia > > &media)
{
    Q_FOREACH(const QSharedPointer<PmcMedia> &m, media) {
        d->pmcMediaByType[m->type()].append(m);
    }
}

void MediaLibraryWrapperCache::removeMediaRef(QSharedPointer<PmcMedia> media)
{
    d->pmcMediaByType[media->type()].removeOne(media);
}

void MediaLibraryWrapperCache::saveNewAlbums(const QList<QSharedPointer<PmcAlbum> > &albums)
{
    d->pmcAlbums.append(albums);
    Q_FOREACH(QSharedPointer<PmcAlbum> album, albums) {
        connect(album.data(), &PmcAlbum::removeRefs, this, &MediaLibraryWrapperCache::removeAlbumRef);
    }
}

void MediaLibraryWrapperCache::removeAlbumRef()
{
    QSharedPointer<PmcAlbum> album = QSharedPointer<PmcAlbum>(static_cast<PmcAlbum*>(sender()));
    d->pmcAlbums.removeOne(album);
}

void MediaLibraryWrapperCache::saveNewArtists(const QList<QSharedPointer<PmcArtist> > &artists)
{
    d->pmcArtists.append(artists);
    Q_FOREACH(QSharedPointer<PmcArtist> artist, artists) {
        connect(artist.data(), &PmcArtist::removeRefs, this, &MediaLibraryWrapperCache::removeArtistRef);
    }
}

void MediaLibraryWrapperCache::removeArtistRef()
{
    QSharedPointer<PmcArtist> artist = QSharedPointer<PmcArtist>(static_cast<PmcArtist*>(sender()));
    d->pmcArtists.removeOne(artist);
}

QList< QSharedPointer< PmcAlbum > > MediaLibraryWrapperCache::getAlbums() const
{
    return d->pmcAlbums;
}

QList< QSharedPointer< PmcArtist > > MediaLibraryWrapperCache::getArtists() const
{
    return d->pmcArtists;
}

QList< QSharedPointer< PmcMedia > > MediaLibraryWrapperCache::getMedia(const QString& type)
{
    return d->pmcMediaByType[type];
}
