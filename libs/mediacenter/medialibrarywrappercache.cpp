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

#include "medialibrary.h"

#include "pmcmedia.h"
#include "pmcalbum.h"
#include "pmcartist.h"

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
    connect(parent, SIGNAL(newMedia(QList<QSharedPointer<PmcMedia> >)),
            SLOT(saveNewMedia(QList<QSharedPointer<PmcMedia> >)));
    connect(parent, SIGNAL(newAlbums(QList<QSharedPointer<PmcAlbum> >)),
            SLOT(saveNewAlbums(QList<QSharedPointer<PmcAlbum> >)));
    connect(parent, SIGNAL(newArtists(QList<QSharedPointer<PmcArtist> >)),
            SLOT(saveNewArtists(QList<QSharedPointer<PmcArtist> >)));
}


MediaLibraryWrapperCache::~MediaLibraryWrapperCache()
{
    delete d;
}

void MediaLibraryWrapperCache::saveNewMedia(const QList< QSharedPointer< PmcMedia > > &media)
{
    foreach(const QSharedPointer<PmcMedia> &m, media) {
        d->pmcMediaByType[m->type()].append(m);
    }
}

void MediaLibraryWrapperCache::saveNewAlbums(const QList<QSharedPointer<PmcAlbum> > &albums)
{
    d->pmcAlbums.append(albums);
}

void MediaLibraryWrapperCache::saveNewArtists(const QList<QSharedPointer<PmcArtist> > &artists)
{
    d->pmcArtists.append(artists);
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
