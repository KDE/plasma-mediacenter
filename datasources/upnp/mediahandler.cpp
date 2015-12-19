/***********************************************************************************
 *   Copyright 2015 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2015 Bhushan Shah    <bshah@kde.org>                                *
 *   Copyright 2015 Ashish Bansal   <bansal.ashish096@gmail.com>                   *
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

#include "mediahandler.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <QVariant>
#include <QUrl>
#include <QMimeDatabase>
#include <QMimeType>
#include <QDebug>

MediaHandler::MediaHandler(MediaLibrary *mediaLibrary, QObject* parent)
    : QObject(parent)
    , mMediaLibrary(mediaLibrary)
{
}

void MediaHandler::handleResult(QHash<int, QString> properties)
{
        QString rawUrl = properties.value(MediaCenter::MediaUrlRole);
        QString mediaType = properties.value(MediaCenter::MimeTypeRole);

        QHash <int, QVariant> values;
        values.insert(Qt::DisplayRole, QVariant(properties.value(Qt::DisplayRole)));
        values.insert(MediaCenter::MediaUrlRole, QVariant(rawUrl));
        values.insert(MediaCenter::MediaTypeRole, QVariant(mediaType));

        if (mediaType == "audio") {
            values.insert(MediaCenter::DurationRole, properties.value(MediaCenter::DurationRole));
            values.insert(MediaCenter::ArtistRole, properties.value(MediaCenter::ArtistRole));
            values.insert(MediaCenter::AlbumRole, properties.value(MediaCenter::AlbumRole));
            values.insert(MediaCenter::AlbumArtistRole, properties.value(MediaCenter::AlbumArtistRole));
        } else if (mediaType == "video") {
            values.insert(MediaCenter::DurationRole, properties.value(MediaCenter::DurationRole));
        }
        mMediaLibrary->updateMedia(values);
}
