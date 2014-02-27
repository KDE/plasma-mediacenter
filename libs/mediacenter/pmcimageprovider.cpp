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

#include "pmcimageprovider.h"
#include "pmcimagecache.h"
#include "singletonfactory.h"

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
    QImage image = SingletonFactory::instanceFor<PmcImageCache>()->getImage(id);

    if (size) {
        *size = image.size();
    }

    QSize reqSize = requestedSize;
    if (!image.size().isEmpty() && reqSize.isValid()) {
        if (reqSize.width() == 0)
            reqSize.setWidth(reqSize.height()*image.size().width()/image.size().height());
        else if (reqSize.height() == 0) {
            reqSize.setHeight(reqSize.width()*image.size().height()/image.size().width());
        }
    }

    return reqSize.isValid() ? image.scaled(reqSize, Qt::KeepAspectRatioByExpanding) : image;
}

