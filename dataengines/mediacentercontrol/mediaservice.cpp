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

#include "mediaservice.h"
#include "mediajob.h"

MediaService::MediaService(Media* media, QObject* parent)
    : Plasma::Service(parent), m_media(media)
{
    setName("mediacentercontrol");
    if (m_media) {
        setDestination(m_media->name());
    }
    connect(m_media, SIGNAL(mediaDataUpdated()), SLOT(enableMediaOperations()));
    enableMediaOperations();
}

void MediaService::enableMediaOperations()
{
    if (m_media) {
        setOperationEnabled("play", true);
        setOperationEnabled("pause", true);
        setOperationEnabled("stop", m_media->canStop());
        setOperationEnabled("next", m_media->canGoNext());
        setOperationEnabled("previous", m_media->canGoPrevious());
        setOperationEnabled("volume", m_media->canSetVolume());
        setOperationEnabled("seek", m_media->canSeek());
        setOperationEnabled("mediaProgress", m_media->canUpdateMediaProgress());
        setOperationEnabled("dirtyCheck", m_media->getDirty());
    }
}

Plasma::ServiceJob* MediaService::createJob(const QString &operation, QMap<QString, QVariant> &parameters)
{
    return new MediaJob(m_media, destination(), operation, parameters, this);
}

#include "mediaservice.moc"