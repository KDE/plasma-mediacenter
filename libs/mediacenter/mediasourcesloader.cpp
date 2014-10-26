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

#include <KPluginTrader>
#include <KPluginInfo>
#include <QDebug>
#include <QApplication>

MediaSourcesLoader::MediaSourcesLoader(QObject* parent): QObject(parent)
{

}

void MediaSourcesLoader::load()
{
    KPluginInfo::List pluginInfo = KPluginTrader::self()->query("plasma/mediacenter/mediasources");
    if (pluginInfo.isEmpty()) {
        qWarning() << "no available media sources";
    }

    Q_FOREACH (const KPluginInfo &info, pluginInfo) {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        const QVariantList args = QVariantList() << loader.metaData().toVariantMap();
        if(factory)
        {
            MediaCenter::AbstractMediaSource *mediaSource = factory->create<MediaCenter::AbstractMediaSource>(0, args);
            if (mediaSource) {
                mediaSource->setMediaLibrary(SingletonFactory::instanceFor<MediaLibrary>());
                mediaSource->start();
                connect(QApplication::instance(), SIGNAL(destroyed(QObject*)), mediaSource, SLOT(quit()));
                qDebug() << "created instance for media source" << info.name();
            } else {
                qDebug() << "Could not create a instance for the media source " << info.name();
            }
        }

    }
}
