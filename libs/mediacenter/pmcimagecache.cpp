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

#include "pmcimagecache.h"

#include <KDE/KGlobal>
#include <KDE/KImageCache>

#include <QtCore/QCoreApplication>
#include <QDebug>

class PmcImageCache::Singleton
{
public:
    PmcImageCache q;
};

K_GLOBAL_STATIC( PmcImageCache::Singleton, singleton )

PmcImageCache *PmcImageCache::instance()
{
    return &( singleton->q );
}

PmcImageCache::~PmcImageCache()
{
}

PmcImageCache::PmcImageCache()
{
    m_imageCache = new KImageCache("plasma_engine_preview", 10485760);
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
