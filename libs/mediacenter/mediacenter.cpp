/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "mediacenter.h"

#include <QFileInfo>

#include <KMimeType>
#include <KUrl>

#include <Solid/Device>
#include <Solid/OpticalDisc>

#include <Phonon/MediaSource>

namespace MediaCenter {

MediaType getType(const QString &media)
{
    QFileInfo info(media);
    if (info.exists()) {
        KMimeType::Ptr mime = KMimeType::findByPath(media);
        if (mime->name().startsWith("image/")) {
            return Picture;
        } else if (mime->name().startsWith("video/")) {
            return Video;
        } else if (mime->name().startsWith("audio/")) {
            return Audio;
        } else {
            return Invalid;
        }
    } else { // it is not a file
        Solid::Device genericDevice(media);
        if (!genericDevice.isValid()) {
            return Invalid;
        }
        Solid::OpticalDisc *opticalDisc = genericDevice.as<Solid::OpticalDisc>();
        if (!opticalDisc) {
            return Invalid;
        }

        return OpticalDisc;
    }
}

Media mediaFromMediaSource(const Phonon::MediaSource &source)
{
    if (source.url().isValid()) {
        Media media;
        media.second = KUrl(source.url()).path();
        media.first = getType(media.second);
        if (media.first == MediaCenter::Invalid) {
            media.second = "";
            return media;
        }
        return media;
    }

    if (!source.fileName().isEmpty()) {
        Media media;
        media.second = source.fileName();
        media.first = getType(media.second);
        if (media.first == MediaCenter::Invalid) {
            media.second = "";
            return media;
        }
        return media;
    }

    if (!source.deviceName().isEmpty()) {
        Media media;
        media.second = source.deviceName();
        media.first = MediaCenter::OpticalDisc;
        return media;
    }

    Media media;
    media.first = Invalid;
    return media;
}

} // MediaCenter namespace
