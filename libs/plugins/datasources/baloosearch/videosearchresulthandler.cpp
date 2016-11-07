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

#include "videosearchresulthandler.h"

#include <medialibrary.h>
#include <kmediacollection.h>

#include <baloo/file.h>
#include <baloo/resultiterator.h>

#include <QHash>
#include <QUrl>

VideoSearchResultHandler::VideoSearchResultHandler(MediaLibrary* mediaLibrary,
                                                   QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
{

}

QString VideoSearchResultHandler::supportedMediaType() const
{
    return "Video";
}

QString VideoSearchResultHandler::supportedMimeType() const
{
    return "video";
}

void VideoSearchResultHandler::handleResultImpl(
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

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.path()).toString(), vals);
}
