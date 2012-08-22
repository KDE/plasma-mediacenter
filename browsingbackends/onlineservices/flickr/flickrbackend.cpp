/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "flickrbackend.h"

#include "flickrmodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(FlickrBackend)

FlickrBackend::FlickrBackend(QObject* parent, const QVariantList& args)
    : MediaCenter::AbstractBrowsingBackend(parent, args)
{

}


FlickrBackend::~FlickrBackend()
{

}

void FlickrBackend::init()
{
    setModel(new FlickrModel(this));
}

bool FlickrBackend::goOneLevelUp()
{
    return MediaCenter::AbstractBrowsingBackend::goOneLevelUp();
}

QString FlickrBackend::backendCategory() const
{
    return "image";
}

void FlickrBackend::search(const QString& searchTerm)
{
    qobject_cast<FlickrModel*>(model())->query(searchTerm);
}

bool FlickrBackend::supportsSearch() const
{
    return true;
}
