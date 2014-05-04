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

#include <baloo/filefetchjob.h>
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

    Baloo::FileFetchJob* job = new Baloo::FileFetchJob(resultIterator.url().toLocalFile());
    connect(job, SIGNAL(fileReceived(Baloo::File)),
            this, SLOT(slotFileReceived(Baloo::File)));

    job->start();
}

void VideoSearchResultHandler::slotFileReceived(const Baloo::File& file)
{
    QHash<int, QVariant> values;

    const int duration = file.property(KFileMetaData::Property::Duration).toInt();
    if (duration) {
        values.insert(MediaCenter::DurationRole, duration);
    }

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.url()).toString(), values);
}
