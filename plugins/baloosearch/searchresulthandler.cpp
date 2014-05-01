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

#include "searchresulthandler.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <baloo/resultiterator.h>

#include <QFileInfo>
#include <QDateTime>

SearchResultHandler::SearchResultHandler(MediaLibrary *mediaLibrary, QObject* parent)
    : QObject(parent)
    , m_mediaLibrary(mediaLibrary)
{
}

void SearchResultHandler::handleResult(Baloo::ResultIterator& resultIterator)
{
    while (resultIterator.next()) {
        //First collect common information
        QHash<int, QVariant> values;

        values.insert(Qt::DisplayRole, resultIterator.text());
        values.insert(Qt::DecorationRole, resultIterator.icon());
        values.insert(MediaCenter::MediaTypeRole, supportedMediaType().toLower());
        values.insert(MediaCenter::MediaUrlRole, resultIterator.url());

        //HACK: This is a workaround as Baloo does not provide creation or
        // modification date/time through KFileMetaData::Property
        values.insert(MediaCenter::CreatedAtRole,
                      QFileInfo(resultIterator.url().toLocalFile()).created());

        m_mediaLibrary->updateMedia(values);

        //Now collect information specific to this media type
        handleResultImpl(resultIterator);
    }
}
