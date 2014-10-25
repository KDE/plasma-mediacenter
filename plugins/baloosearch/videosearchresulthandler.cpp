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

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <baloo/file.h>
#include <baloo/resultiterator.h>

#include <QHash>

VideoSearchResultHandler::VideoSearchResultHandler(MediaLibrary* mediaLibrary,
                                                   QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
{

}

QString VideoSearchResultHandler::supportedMediaType() const
{
    return "Video";
}

void VideoSearchResultHandler::handleResultImpl(
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

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.path()).toString(), vals);
}
