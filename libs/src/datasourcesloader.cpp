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

#include "datasourcesloader.h"
#include "medialibrary.h"
#include "abstractdatasource.h"

#include "singletonfactory.h"

#include <KPluginTrader>
#include <KPluginInfo>
#include <QDebug>
#include <QApplication>

DataSourcesLoader::DataSourcesLoader(QObject* parent): QObject(parent)
{

}

void DataSourcesLoader::load()
{
    KPluginInfo::List pluginInfo = KPluginTrader::self()->query("plasma/mediacollection/datasources");
    if (pluginInfo.isEmpty()) {
        qWarning() << "no available data sources";
    }

    Q_FOREACH (const KPluginInfo &info, pluginInfo) {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        const QVariantList args = QVariantList() << loader.metaData().toVariantMap();
        if(factory)
        {
            KMediaCollection::AbstractDataSource *dataSource = factory->create<KMediaCollection::AbstractDataSource>(0, args);
            if (dataSource) {
                dataSource->setMediaLibrary(SingletonFactory::instanceFor<MediaLibrary>());
                dataSource->start();
                connect(QApplication::instance(), SIGNAL(destroyed(QObject*)), dataSource, SLOT(quit()));
                qDebug() << "created instance for data source" << info.name();
            } else {
                qDebug() << "Could not create a instance for the data source " << info.name();
            }
        }

    }
}
