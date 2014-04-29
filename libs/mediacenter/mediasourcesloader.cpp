/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
 *                                                                                 *
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

#include "mediasourcesloader.h"
#include "medialibrary.h"
#include "abstractmediasource.h"

#include "singletonfactory.h"

#include <KPluginInfo>
#include <QDebug>

MediaSourcesLoader::MediaSourcesLoader(QObject* parent): QObject(parent)
{

}

MediaSourcesLoader::~MediaSourcesLoader()
{

}

void MediaSourcesLoader::load()
{
    KService::List services = MediaCenter::AbstractMediaSource::availableMediaSourcePlugins();
    KPluginInfo::List pluginInfo = KPluginInfo::fromServices(services);

    Q_FOREACH (const KPluginInfo &info, pluginInfo) {
        KService::Ptr service = info.service();
        if (!service) {
            qDebug() << "Could not get the service for media source " << info.name();
            continue;
        }
        const QString key = service->library();
        QString errorMessage;

        MediaCenter::AbstractMediaSource *mediaSource =
        service->createInstance<MediaCenter::AbstractMediaSource>(
            0, QVariantList() << service->storageId(), &errorMessage);

        if (mediaSource) {
            mediaSource->setParent(this);
            mediaSource->setMediaLibrary(SingletonFactory::instanceFor<MediaLibrary>());
            mediaSource->start();
        } else {
            qDebug() << "Could not create a instance for the media source " << info.name() << errorMessage;
        }
    }

}
