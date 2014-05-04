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

/* Part of this code was taken from project Taglib, originally licensed under
 * the BSD-2-clause license. You can find the original version at
 * https://github.com/taglib/taglib/tree/master/examples
*/

#include "pmccoverartprovider.h"
#include "pmcimagecache.h"
#include "singletonfactory.h"

#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>

#include <QtCore/QUrl>

const char *PmcCoverArtProvider::identificationString = "coverart";

PmcCoverArtProvider::PmcCoverArtProvider()
    : QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
}

QImage PmcCoverArtProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    const QString identification = id.split(':').at(0);
    const QString url = id.section(':', 1);

    if (!id.startsWith("file:")) {
        return SingletonFactory::instanceFor<PmcImageCache>()->getImage(id);
    }

    //Usually, that ^ is the job of PmcCoverArtProvider, to just get the image
    //from the cache and return it. However, MusicStats.qml also wants a cover
    //from the real music file, and that is when it uses "file:"
    //TODO: This is cover extraction code and should be moved out of this. Or,
    //nuked altogether.
    const QString file = QUrl(url).toLocalFile();
    TagLib::MPEG::File f(file.toUtf8().constData());

    if(!f.isValid() || !f.ID3v2Tag())
        return QImage();

    TagLib::ID3v2::FrameList l = f.ID3v2Tag()->frameList("APIC");
    QImage image;

    if(l.isEmpty()) {
        return image;
    }
    TagLib::ID3v2::AttachedPictureFrame *frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
    image.loadFromData((const uchar *) frame->picture().data(), frame->picture().size());

    if (size) {
        *size = image.size();
    }

    addAlbumCoverToCacheIfMissing(f, image);
    return image;
}

void PmcCoverArtProvider::addAlbumCoverToCacheIfMissing(TagLib::MPEG::File& f, const QImage& image) const
{
    QString albumName(f.ID3v2Tag()->album().toCString());
    PmcImageCache *imageCache = SingletonFactory::instanceFor<PmcImageCache>();

    if(!imageCache->containsAlbumCover(albumName)) {
        imageCache->addAlbumCover(albumName, image);
    }
}

QString PmcCoverArtProvider::qmlImageUriForAlbumCover(const QString& albumName)
{
    return QString("image://%1/%2").arg(identificationString,
                                        PmcImageCache::imageIdForAlbumCover(albumName));
}

QString PmcCoverArtProvider::qmlImageUriForArtistCover(const QString& artistName)
{
    return QString("image://%1/%2").arg(identificationString,
                                        PmcImageCache::imageIdForArtistCover(artistName));
}

QString PmcCoverArtProvider::qmlImageUriForMediaFileCover(const QString& mediaFileUrl)
{
    return QString("image://%1/%2").arg(identificationString, PmcImageCache::imageIdForMediaFileCover(mediaFileUrl));
}
