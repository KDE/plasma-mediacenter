/***********************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef PMCIMAGECACHE_H
#define PMCIMAGECACHE_H

#include <QtDeclarative/QDeclarativeImageProvider>
#include <QtCore/QHash>

#include "mediacenter_export.h"

class KImageCache;

class MEDIACENTER_EXPORT PmcImageCache
{
public:
    class Singleton;

    static PmcImageCache *instance();

    void addImage(const QString& id, const QImage &image);
    bool containsId(const QString &id) const;
    QImage getImage(const QString &id) const;
private:
    PmcImageCache();
    ~PmcImageCache();

    KImageCache *m_imageCache;
};

#endif // PMCIMAGECACHE_H
