/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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


#include "abstractmetadatabackend.h"

#include <modelmetadata.h>
#include <pmcmetadatamodel.h>

AbstractMetadataBackend::AbstractMetadataBackend(QObject* parent, const QVariantList& args)
    : AbstractBrowsingBackend(parent, args)
    , m_busy(false)
{
}

bool AbstractMetadataBackend::initImpl()
{
    return true;
}

bool AbstractMetadataBackend::goOneLevelUp()
{
    return false;
}

bool AbstractMetadataBackend::busy() const
{
    return m_busy;
}

void AbstractMetadataBackend::handleBusySignals ( PmcMetadataModel* emitter )
{
    connect(emitter, SIGNAL(queryStarted()), SLOT(makeBusy()));
    connect(emitter, SIGNAL(queryError(QString)), SLOT(makeFree()));
    connect(emitter, SIGNAL(queryFinished()), SLOT(makeFree()));
}

void AbstractMetadataBackend::makeBusy()
{
    m_busy = true;
    emit busyChanged();
}

void AbstractMetadataBackend::makeFree()
{
    m_busy = false;
    emit busyChanged();
}

void AbstractMetadataBackend::setModel(ModelMetadata* modelMetadata)
{
    PmcMetadataModel *metadataModel = qobject_cast<PmcMetadataModel*>(modelMetadata->model());
    if (metadataModel) {
        connect(metadataModel, SIGNAL(queryError(QString)), SIGNAL(error(QString)));
    }

    MediaCenter::AbstractBrowsingBackend::setModel(modelMetadata);
}

#include "abstractmetadatabackend.moc"
