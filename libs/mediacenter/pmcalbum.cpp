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

#include "pmcalbum.h"

#include "album.h"

class PmcAlbum::Private
{
public:
    QSharedPointer< Album > album;
};

PmcAlbum::PmcAlbum(const QSharedPointer< Album >& album, QObject* parent)
    : QObject(parent)
    , d(new Private())
{
    d->album = album;
    connect(d->album.data(), &Album::updated, this, &PmcAlbum::updated);
}


PmcAlbum::~PmcAlbum()
{

}

const QString& PmcAlbum::name() const
{
    return d->album->name();
}

QString PmcAlbum::albumArtist() const
{
    return d->album->albumArtist();
}

int PmcAlbum::mediaCount() const
{
    return d->album->media().size();
}

void PmcAlbum::removeMedia(const QSharedPointer<Media> &media)
{
    d->album->removeMedia(media);
    if (mediaCount() == 0) {
        emit removeRefs();
    }
}
