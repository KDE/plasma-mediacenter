/***********************************************************************************
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

#include "pmcimagecache.h"

#include <QCoreApplication>

const char *PmcImageCache::fileIdentification = "mediafile:";
const char *PmcImageCache::albumIdentification = "album:";
const char *PmcImageCache::artistIdentification = "artist:";

PmcImageCache::PmcImageCache()
{
    m_imageCache = new KImageCache("plasma_engine_preview", 209715200);
}

void PmcImageCache::addImage(const QString& id, const QImage& image)
{
    m_imageCache->insertImage(id, image);
}

bool PmcImageCache::containsImageWithId(const QString& id) const
{
    return m_imageCache->contains(id);
}

QImage PmcImageCache::getImage(const QString& id) const
{
    QImage image;
    m_imageCache->findImage(id, &image);
    return image;
}

void PmcImageCache::addAlbumCover(const QString& albumName, const QImage& image)
{
    if (!image.isNull()) {
        addImage(imageIdForAlbumCover(albumName), image);
    }
}

void PmcImageCache::addArtistCover(const QString& artistName, const QImage& image)
{
    if (!image.isNull()) {
        addImage(imageIdForArtistCover(artistName), image);
    }
}

void PmcImageCache::addMediaFileCover(const QString& mediaFileUrl, const QImage& image)
{
    if (!image.isNull()) {
        addImage(imageIdForMediaFileCover(mediaFileUrl), image);
    }
}

QString PmcImageCache::imageIdForAlbumCover(const QString& albumOrArtistName)
{
    return QString(albumOrArtistName).prepend(albumIdentification);
}

QString PmcImageCache::imageIdForArtistCover(const QString& albumOrArtistName)
{
    return QString(albumOrArtistName).prepend(artistIdentification);
}

QString PmcImageCache::imageIdForMediaFileCover(const QString& mediaFileUrl)
{
    return QString(mediaFileUrl).prepend(fileIdentification);
}

bool PmcImageCache::containsAlbumCover(const QString& albumName) const
{
    return containsImageWithId(imageIdForAlbumCover(albumName));
}

bool PmcImageCache::containsArtistCover(const QString& artistName) const
{
    return containsImageWithId(imageIdForArtistCover(artistName));
}

bool PmcImageCache::containsMediaFileCover(const QString& mediaFileUrl) const
{
    return containsImageWithId(imageIdForMediaFileCover(mediaFileUrl));
}
