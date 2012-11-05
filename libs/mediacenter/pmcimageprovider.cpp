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

#include "pmcimageprovider.h"
#include "pmcimagecache.h"

#include <QDebug>

const char *PmcImageProvider::identificationString = "pmcimage";

PmcImageProvider::PmcImageProvider(): QDeclarativeImageProvider(Image)
{

}

PmcImageProvider::~PmcImageProvider()
{

}

QImage PmcImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    QImage image = PmcImageCache::instance()->getImage(id);

    if (size) {
        *size = image.size();
    }

    QSize reqSize = requestedSize;
    if (!image.size().isEmpty() && reqSize.isValid()) {
        if (reqSize.width() == 0)
            reqSize.setWidth(reqSize.height()/image.size().height()*image.size().width());
        else if (reqSize.height() == 0) {
            reqSize.setHeight(reqSize.width()/image.size().width()*image.size().height());
        }
    }

    return reqSize.isValid() ? image.scaled(reqSize, Qt::KeepAspectRatioByExpanding) : image;
}

