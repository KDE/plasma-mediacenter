/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
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
#include "flickr.h"
#include "flickrinterface.h"

// KDE
#include <kio/job.h>

FlickrEngine::FlickrEngine(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args),
m_interface(new FlickrInterface(this))
{
    connect (m_interface, SIGNAL(result(QString, Plasma::DataEngine::Data)),
             this, SLOT(slotSourceFromResult(QString,  Plasma::DataEngine::Data)));
}

FlickrEngine::~FlickrEngine()
{
}

bool FlickrEngine::sourceRequestEvent(const QString &name)
{
    m_interface->query(name);
    return true;
}

void FlickrEngine::slotSourceFromResult(const QString &searchTerm, const Plasma::DataEngine::Data &image)
{
    setData(searchTerm, image);
}

K_EXPORT_PLASMA_DATAENGINE(flickr, FlickrEngine)
