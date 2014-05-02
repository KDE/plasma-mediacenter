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

#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <baloo/resultiterator.h>
#include <baloo/filefetchjob.h>
#include <baloo/file.h>

#include <QDateTime>

ImageSearchResultHandler::ImageSearchResultHandler(MediaLibrary* mediaLibrary, QObject* parent)
    : SearchResultHandler(mediaLibrary, parent)
{
}

QString ImageSearchResultHandler::supportedMediaType() const
{
    return "Image";
}

void ImageSearchResultHandler::handleResultImpl(const Baloo::ResultIterator& resultIterator)
{
    Baloo::FileFetchJob* job = new Baloo::FileFetchJob(resultIterator.url().toLocalFile());
    connect(job, SIGNAL(fileReceived(Baloo::File)),
            this, SLOT(slotFileReceived(Baloo::File)));

    job->start();
}

void ImageSearchResultHandler::slotFileReceived(const Baloo::File &file)
{
    //Properties that signify the actual date/time the image was taken by the
    //camera
    QList<KFileMetaData::Property::Property> properties;
    properties << KFileMetaData::Property::PhotoDateTimeOriginal
               << KFileMetaData::Property::ImageDateTime;

    QDateTime created;
    Q_FOREACH(KFileMetaData::Property::Property property, properties) {
        created = file.property(property).toDateTime();
        if (created.isValid()) {
            break;
        }
    }

    QHash<int, QVariant> values;
    if (created.isValid()) {
        values.insert(MediaCenter::CreatedAtRole, created);
    }

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.url()).toString(), values);
}
