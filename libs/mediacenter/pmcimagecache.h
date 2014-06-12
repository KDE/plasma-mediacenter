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

#ifndef PMCIMAGECACHE_H
#define PMCIMAGECACHE_H

#include <QtCore/QHash>

#include <KImageCache>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcImageCache : public QObject
{
    Q_OBJECT
public:
    static const char *fileIdentification;
    static const char *albumIdentification;
    static const char *artistIdentification;

    PmcImageCache();

    void addImage(const QString& id, const QImage &image);
    QImage getImage(const QString &id) const;
    bool containsImageWithId(const QString &id) const;

    void addAlbumCover(const QString &albumName,
                               const QImage &image);
    void addArtistCover(const QString &artistName,
                               const QImage &image);
    void addMediaFileCover(const QString &mediaFileUrl, const QImage &image);

    bool containsAlbumCover(const QString &albumName) const;
    bool containsArtistCover(const QString &artistName) const;
    bool containsMediaFileCover(const QString& mediaFileUrl) const;

    static QString imageIdForAlbumCover(const QString& albumOrArtistName);
    static QString imageIdForArtistCover(const QString& albumOrArtistName);
    static QString imageIdForMediaFileCover(const QString &mediaFileUrl);

private:
    KImageCache *m_imageCache;
};

#endif // PMCIMAGECACHE_H
