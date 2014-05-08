/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>          `                     *
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

#include "baloosearchmediasource.h"

#include "imagesearchresulthandler.h"
#include "videosearchresulthandler.h"
#include "audiosearchresulthandler.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/mediacenter.h>

#include <baloo/query.h>
#include <baloo/result.h>
#include <baloo/filefetchjob.h>
#include <baloo/file.h>

#include <QDebug>
#include <QTimer>


MEDIACENTER_EXPORT_MEDIASOURCE(BalooSearchMediaSource, "baloosearch.json")

BalooSearchMediaSource::BalooSearchMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{
}

void BalooSearchMediaSource::run()
{
    QTimer::singleShot(0, this, SLOT(startQuerying()));

    exec();
}

void BalooSearchMediaSource::startQuerying()
{
    MediaLibrary *mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();

    QList<SearchResultHandler*> searchResultHandlerList;
    searchResultHandlerList << new ImageSearchResultHandler(mediaLibrary, this)
                            << new VideoSearchResultHandler(mediaLibrary, this)
                            << new AudioSearchResultHandler(mediaLibrary, this);

    Q_FOREACH(SearchResultHandler* searchResultHandler, searchResultHandlerList) {
        m_searchResultHandlers.insert(searchResultHandler->supportedMediaType(), searchResultHandler);
    }

    Q_FOREACH(const QString &type, m_searchResultHandlers.keys()) {
        queryForMediaType(type);
    }
}

void BalooSearchMediaSource::queryForMediaType(const QString& type)
{
    Baloo::Query query;
    query.addType(type);

    Baloo::ResultIterator it = query.exec();
    SearchResultHandler *handler = m_searchResultHandlers.value(type);
    handler->handleResult(it);
}

void BalooSearchMediaSource::fetchUrlDetails(const QUrl& url)
{
    Baloo::FileFetchJob* job = new Baloo::FileFetchJob(url.toLocalFile());
    connect(job, SIGNAL(fileReceived(Baloo::File)),
            this, SLOT(slotFileReceived(Baloo::File)));

    job->start();
}

void BalooSearchMediaSource::slotFileReceived(const Baloo::File& file)
{
    QHash<int, QVariant> values;

    //file.url() provides a local path, convert that to file:// url
    const QString fileUrl = QUrl::fromLocalFile(file.url()).toString();
    values.insert(MediaCenter::MediaUrlRole, fileUrl);

    const QString title = file.property(KFileMetaData::Property::Title).toString();
    values.insert(Qt::DisplayRole, title);

    values.insert(MediaCenter::ArtistRole, file.property(KFileMetaData::Property::Artist).toString());
    values.insert(MediaCenter::AlbumRole, file.property(KFileMetaData::Property::Album).toString());
    values.insert(MediaCenter::DurationRole, file.property(KFileMetaData::Property::Duration).toInt());

    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
}

#include "baloosearchmediasource.moc"
