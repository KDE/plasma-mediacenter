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

#include <medialibrary.h>
#include <kmediacollection.h>

#include <baloo/resultiterator.h>

#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#include <QUrl>
#include <QMimeDatabase>
#include <KFileMetaData/UserMetaData>

SearchResultHandler::SearchResultHandler(MediaLibrary *mediaLibrary, QObject* parent)
    : QObject(parent)
    , m_mediaLibrary(mediaLibrary)
{
}

void SearchResultHandler::handleResult(QString &filePath)
{
    QHash<int, QVariant> values;
    const QUrl url = QUrl::fromLocalFile(filePath);
    KFileMetaData::UserMetaData md(filePath);

    values.insert(Qt::DisplayRole, QVariant(url.fileName()));
    values.insert(Qt::DecorationRole, QVariant(QMimeDatabase().mimeTypeForFile(filePath).iconName()));
    values.insert(KMediaCollection::MediaTypeRole, QVariant(supportedMediaType().toLower()));
    values.insert(KMediaCollection::MediaUrlRole, QVariant(url.toString()));
    values.insert(KMediaCollection::RatingRole, QVariant(md.rating()));

    //HACK: This is a workaround as Baloo does not provide creation or
    // modification date/time through KFileMetaData::Property
    values.insert(KMediaCollection::CreatedAtRole,
            QVariant(QFileInfo(filePath).created()));

    //Now collect information specific to this media type
    handleResultImpl(filePath, values);

}
