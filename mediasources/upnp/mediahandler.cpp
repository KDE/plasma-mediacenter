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
        QHash<int, QVariant> values;
        QString rawUrl = properties.value(0);
        QUrl url (rawUrl);
        QMimeType mimeType = QMimeDatabase().mimeTypeForUrl(rawUrl);
        QString mediaType = mimeType.name();
        mediaType = mediaType.split("/").at(0).toLower();
        if(!QString::compare(mediaType, "text", Qt::CaseInsensitive)){
            return;
        }
        values.insert(Qt::DisplayRole, QVariant(properties.value(1)));
        values.insert(Qt::DecorationRole, QVariant(mimeType.iconName()));
        values.insert(MediaCenter::MediaUrlRole, QVariant(url.toString()));
        values.insert(MediaCenter::MediaTypeRole, QVariant(mediaType.toLower()));

        if(!QString::compare(mediaType, "Audio", Qt::CaseInsensitive)){
//             values.insert(MediaCenter::DurationRole, properties.value(2));
            values.insert(MediaCenter::ArtistRole, properties.value(3));
            values.insert(MediaCenter::AlbumRole, properties.value(4));
            values.insert(MediaCenter::AlbumArtistRole, properties.value(5));
        }else if(!QString::compare(mediaType, "Video", Qt::CaseInsensitive)){
//             values.insert(MediaCenter::DurationRole, properties.value(2));
        }else if(!QString::compare(mediaType, "Image", Qt::CaseInsensitive)){

        }
        mMediaLibrary->updateMedia(rawUrl, values);
}
