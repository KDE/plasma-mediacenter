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

#include <mediacenter.h>
#include <medialibrary.h>

#include <baloo/file.h>
#include <baloo/resultiterator.h>

AudioSearchResultHandler::AudioSearchResultHandler(MediaLibrary* mediaLibrary,
                                                   QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
{
}

QString AudioSearchResultHandler::supportedMediaType() const
{
    return "Audio";
}

void AudioSearchResultHandler::handleResultImpl(
    const Baloo::ResultIterator& resultIterator,
    const QHash< int, QVariant >& values)
{
    m_mediaLibrary->updateMedia(values);

    Baloo::File file(resultIterator.filePath());
    file.load();

    QHash<int, QVariant> vals;

    const int duration = file.property(KFileMetaData::Property::Duration).toInt();
    if (duration) {
        vals.insert(MediaCenter::DurationRole, duration);
    }

    const QString title = file.property(KFileMetaData::Property::Title).toString();
    if (!title.isEmpty()) {
        vals.insert(Qt::DisplayRole, title);
    }

    vals.insert(MediaCenter::ArtistRole, file.property(KFileMetaData::Property::Artist));
    vals.insert(MediaCenter::AlbumRole, file.property(KFileMetaData::Property::Album));

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.path()).toString(), vals);
}
