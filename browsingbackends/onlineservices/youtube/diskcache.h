/***********************************************************************************
 *   Copyright Flavio Tordini <flavio.tordini@gmail.com>                           *
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

#ifndef DISKCACHE_H
#define DISKCACHE_H

#include <QNetworkDiskCache>

class DiskCache : public QNetworkDiskCache
{
    Q_OBJECT
public:
    explicit DiskCache(QObject *parent = 0);
    QIODevice* prepare(const QNetworkCacheMetaData &metaData);

signals:

public slots:

};

#endif // DISKCACHE_H
