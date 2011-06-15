/***************************************************************************
 *   Copyright 2011 by Sinny Kumari <ksinny@gmail.com>                     *
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
#include "mediacentercontrol.h"
#include "mediacontainer.h"
#include "media.h"
#include "mediaservice.h"
#include "kdebug.h"

MediaCenterControl::MediaCenterControl(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
{
     Q_UNUSED(args);
}

void MediaCenterControl::init()
{
    MediaContainer *source = new MediaContainer(this);
    source->setObjectName("MediaStatus");
    addSource(source);
}

Plasma::Service* MediaCenterControl::serviceForSource(const QString& source)
{
    MediaContainer* container = qobject_cast<MediaContainer*>(containerForSource(source));
    if (container) {
        return container->service(this);
    } else {
        return DataEngine::serviceForSource(source);
    }
}

bool MediaCenterControl::sourceRequestEvent(const QString& source)
{
    kDebug() << "Source" << source;
    if (source != '\0') {
     
        return true;
    }
    return false;
}


K_EXPORT_PLASMA_DATAENGINE(org.kde.mediacentercontrol, MediaCenterControl)

#include "mediacentercontrol.moc"