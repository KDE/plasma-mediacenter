/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#include "audiosearchresulthandler.h"

#include <kmediacollection.h>
#include <medialibrary.h>

#include <baloo/file.h>
#include <baloo/resultiterator.h>

#include <QUrl>

AudioSearchResultHandler::AudioSearchResultHandler(MediaLibrary* mediaLibrary,
                                                   QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
{
}

QString AudioSearchResultHandler::supportedMediaType() const
{
    return "Audio";
}

QString AudioSearchResultHandler::supportedMimeType() const
{
    return "audio";
}

void AudioSearchResultHandler::handleResultImpl(
    const QString& filePath,
    const QHash< int, QVariant >& values)
{
    m_mediaLibrary->updateMedia(values);

    Baloo::File file(filePath);
    file.load();

    QHash<int, QVariant> vals;

    const int duration = file.property(KFileMetaData::Property::Duration).toInt();
    if (duration) {
        vals.insert(KMediaCollection::DurationRole, duration);
    }

    const QString title = file.property(KFileMetaData::Property::Title).toString();
    if (!title.isEmpty()) {
        vals.insert(Qt::DisplayRole, title);
    }

    vals.insert(KMediaCollection::ArtistRole, file.property(KFileMetaData::Property::Artist));
    vals.insert(KMediaCollection::AlbumRole, file.property(KFileMetaData::Property::Album));
    vals.insert(KMediaCollection::AlbumArtistRole, file.property(KFileMetaData::Property::AlbumArtist));

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.path()).toString(), vals);
}
