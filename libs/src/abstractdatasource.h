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

#ifndef ABSTRACTDATASOURCE_H
#define ABSTRACTDATASOURCE_H

#include <QtCore/QThread>
#include <KPluginInfo>

#include "kmediacollection_export.h"

class MediaLibrary;

namespace KMediaCollection {

/**
 * @brief Abstract class for DataSources
 *
 * Example of use:
 * ```
 * KPluginLoader loader(QStringLiteral("plasma/mediacollection/datasources/MySource"));
 * KPluginFactory* factory = loader.factory();
 * const QVariantList args = QVariantList() << loader.metaData().toVariantMap();
 * if(factory)
 * {
 *     KMediaCollection::AbstractDataSource *dataSource = factory->create<KMediaCollection::AbstractDataSource>(0, args);
 *     if (dataSource) {
 *         dataSource->setMediaLibrary(SingletonFactory::instanceFor<MediaLibrary>());
 *         dataSource->start();
 *         connect(QApplication::instance(), SIGNAL(destroyed(QObject*)), dataSource, SLOT(quit()));
 *         qDebug() << "Success";
 *     } else {
 *         qDebug() << "Failure";
 *     }
 * }
 * ```
 */
class KMEDIACOLLECTION_EXPORT AbstractDataSource : public QThread
{
    Q_OBJECT
public:
    /**
     * @brief Constructor
     * @param parent
     * @param QVariantList arguments
     */
    explicit AbstractDataSource(QObject* parent = 0, const QVariantList& = QVariantList());
    ~AbstractDataSource();

    /**
     * @brief Set the MediaLibrary
     * @param mediaLibrary
     */
    void setMediaLibrary(MediaLibrary *mediaLibrary);

protected:
    MediaLibrary *mediaLibrary() const;

private:
    class Private;
    Private * const d;
};
}

#define KMEDIACOLLECTION_EXPORT_DATASOURCE(classname, jsonfile) \
    K_PLUGIN_FACTORY_WITH_JSON( DataSourceFactory, jsonfile, registerPlugin< classname >(); )

#endif // ABSTRACTDATASOURCE_H
