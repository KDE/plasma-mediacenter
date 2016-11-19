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

#ifndef DATASOURCESLOADER_H
#define DATASOURCESLOADER_H

#include <QtCore/QObject>

#include "kmediacollection_export.h"

namespace KMediaCollection {
class AbstractDataSource;
}

/**
 * @brief Used to load all installed datasources.
 *
 * A datasource is loaded if it is found in `$QT_PLUGIN_PATH/plasma/mediacollection/datasources`
 */
class KMEDIACOLLECTION_EXPORT DataSourcesLoader : public QObject
{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param parent
     */
    explicit DataSourcesLoader(QObject* parent = 0);

public Q_SLOTS:
    /**
     * Load all datasources.
     */
    void load();
};

#endif // DATASOURCESLOADER_H
