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

#include <mediacenter/mediacenter.h>
#include <mediacenter/medialibrary.h>

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

void AudioSearchResultHandler::handleResultImpl(const Baloo::ResultIterator& resultIterator)
{
    Baloo::FileFetchJob* job = new Baloo::FileFetchJob(resultIterator.url().toLocalFile());
    connect(job, SIGNAL(fileReceived(Baloo::File)),
            this, SLOT(slotFileReceived(Baloo::File)));

    job->start();
}

void AudioSearchResultHandler::slotFileReceived(const Baloo::File &file)
{
    QHash<int, QVariant> values;

    const int duration = file.property(KFileMetaData::Property::Duration).toInt();
    if (duration) {
        values.insert(MediaCenter::DurationRole, duration);
    }

    const QString title = file.property(KFileMetaData::Property::Title).toString();
    if (!title.isEmpty()) {
        values.insert(Qt::DisplayRole, title);
    }

    values.insert(MediaCenter::ArtistRole, file.property(KFileMetaData::Property::Artist));
    values.insert(MediaCenter::AlbumRole, file.property(KFileMetaData::Property::Album));

    m_mediaLibrary->updateMedia(QUrl::fromLocalFile(file.url()).toString(), values);
}
