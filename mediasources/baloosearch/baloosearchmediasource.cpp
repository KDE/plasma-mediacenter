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
#include <baloo/file.h>

#include <QUrl>
#include <QDebug>
#include <QTimer>
#include <QDBusConnection>
#include <QMimeDatabase>

MEDIACENTER_EXPORT_MEDIASOURCE(BalooSearchMediaSource, "baloosearch.json")

BalooSearchMediaSource::BalooSearchMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{

    m_allowedMimes << "audio" << "image" << "video";

    QDBusConnection conn = QDBusConnection::sessionBus();
    conn.connect(QString(), QLatin1String("/files"), QLatin1String("org.kde"), QLatin1String("changed"), this, SLOT(handleNewFile(QStringList)));

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
        m_searchResultHandlersByMimeType.insert(searchResultHandler->supportedMimeType(), searchResultHandler);
    }

    Q_FOREACH(const QString &type, m_searchResultHandlers.keys()) {
        queryForMediaType(type);
    }
}

void BalooSearchMediaSource::handleNewFile(const QStringList &files)
{
    QMimeDatabase db;
    for (auto file : files)
    {
        const QString fileMimeType = db.mimeTypeForUrl(QUrl::fromLocalFile(file)).name();
        const QString topLevelMimeType = fileMimeType.split('/').at(0);

        if (m_allowedMimes.contains(topLevelMimeType)) {
            SearchResultHandler *handler = m_searchResultHandlersByMimeType.value(topLevelMimeType);
            handler->handleResult(file);
        }
    }
}

void BalooSearchMediaSource::queryForMediaType(const QString& type)
{
    Baloo::Query query;
    query.addType(type);

    Baloo::ResultIterator it = query.exec();
    SearchResultHandler *handler = m_searchResultHandlers.value(type);
    while (it.next()) {
        QString localUrl = it.filePath();
        handler->handleResult(localUrl);
    }

}


#include "baloosearchmediasource.moc"
