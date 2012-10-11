/*
 * This file is part of the KDE project
 *
 * Copyright (C) 2011 Shantanu Tushar <shantanu@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

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
