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

#include "abstractdatasource.h"
#include <QDebug>

using namespace KMediaCollection;

class AbstractDataSource::Private
{
public:
    Private() : mediaLibrary(0) {}
    MediaLibrary* mediaLibrary;
};

AbstractDataSource::AbstractDataSource(QObject* parent, const QVariantList&)
    : QThread(parent), d(new Private)
{
    moveToThread(this);
}

AbstractDataSource::~AbstractDataSource()
{

}

void AbstractDataSource::setMediaLibrary(MediaLibrary* mediaLibrary)
{
    d->mediaLibrary = mediaLibrary;
}

MediaLibrary* AbstractDataSource::mediaLibrary() const
{
    return d->mediaLibrary;
}
