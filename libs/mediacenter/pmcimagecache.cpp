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

#include "pmcimagecache.h"

#include <KDE/KImageCache>

#include <QtCore/QCoreApplication>

PmcImageCache::~PmcImageCache()
{
}

PmcImageCache::PmcImageCache()
{
    m_imageCache = new KImageCache("plasma_engine_preview", 209715200);
}

void PmcImageCache::addImage(const QString& id, const QImage& image)
{
    m_imageCache->insertImage(id, image);
}

bool PmcImageCache::containsId(const QString& id) const
{
    return m_imageCache->contains(id);
}

QImage PmcImageCache::getImage(const QString& id) const
{
    QImage image;
    m_imageCache->findImage(id, &image);
    return image;
}
