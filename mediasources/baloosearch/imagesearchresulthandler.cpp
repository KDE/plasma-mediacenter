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

#include "imagesearchresulthandler.h"
#include <mediacenter/settings.h>

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <baloo/resultiterator.h>
#include <baloo/file.h>

#include <QDateTime>
#include <QUrl>
#include <QDebug>

ImageSearchResultHandler::ImageSearchResultHandler(MediaLibrary* mediaLibrary, QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
    , m_minimumImageSize(Settings().value("minImageWidth", 500).toInt())
{
}

QString ImageSearchResultHandler::supportedMediaType() const
{
    return "Image";
}

void ImageSearchResultHandler::handleResultImpl(
    const Baloo::ResultIterator& resultIterator,
    const QHash< int, QVariant >& values)
{
    const QString fileUrl = values.value(MediaCenter::MediaUrlRole).toUrl().toLocalFile();
    m_initialValuesByUrl.insert(fileUrl, values);

    //We remove the create date as we want to fill it only for images taken with
    //a camera or similar in slotFileReceived(). This is to make sure such
    //photos appear before junk images in the browser.
    m_initialValuesByUrl[fileUrl].remove(MediaCenter::CreatedAtRole);

    Baloo::File file(resultIterator.filePath());
    file.load();

    //Properties that signify the actual date/time the image was taken by the
    //camera
    QList<KFileMetaData::Property::Property> properties;
    properties << KFileMetaData::Property::PhotoDateTimeOriginal
               << KFileMetaData::Property::ImageDateTime;

    auto photoWidth = file.property(KFileMetaData::Property::Width).toULongLong();

    if (!photoWidth || photoWidth < m_minimumImageSize) {
        m_initialValuesByUrl.remove(fileUrl);
        return;
    }

    QDateTime created;
    Q_FOREACH(KFileMetaData::Property::Property property, properties) {
        created = file.property(property).toDateTime();
        if (created.isValid()) {
            break;
        }
    }

    QHash<int, QVariant> vals;
    if (created.isValid()) {
        vals.insert(MediaCenter::CreatedAtRole, created);
    }

    vals.unite(m_initialValuesByUrl.take(fileUrl));

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.path()).toString(),
                                vals);
}
