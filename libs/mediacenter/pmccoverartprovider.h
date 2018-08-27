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

#ifndef PMCCOVERARTPROVIDER_H
#define PMCCOVERARTPROVIDER_H

#include <QQuickImageProvider>

#include "mediacenter_export.h"

namespace TagLib {
    namespace MPEG {
        class File;
    }
}

class MEDIACENTER_EXPORT PmcCoverArtProvider : public QQuickImageProvider
{
public:
    static const char *identificationString;

    PmcCoverArtProvider ();
    QImage requestImage ( const QString& id, QSize* size,
                                  const QSize& requestedSize ) Q_DECL_OVERRIDE;

    static QString qmlImageUriForAlbumCover(const QString &albumName);
    static QString qmlImageUriForArtistCover(const QString &artistName);
    static QString qmlImageUriForMediaFileCover(const QString &mediaFileUrl);

private:
    /**
     * If the cache doesn't yet have an image for this album, add the one we just read
     * from the file
     */
    void addAlbumCoverToCacheIfMissing(TagLib::MPEG::File& f, const QImage& image) const;
};

#endif // PMCCOVERARTPROVIDER_H
