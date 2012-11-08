/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2007 Aaron Seigo <aseigo@kde.org>
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


#include "metadatavideobackend.h"

#include "metadatavideomodel.h"

#include <Nepomuk2/Query/QueryParser>

#include <QDebug>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataVideoBackend)

MetadataVideoBackend::MetadataVideoBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
{
}

MetadataVideoBackend::~MetadataVideoBackend()
{
}

void MetadataVideoBackend::initImpl()
{
    AbstractMetadataBackend::initImpl();
    PmcMetadataModel *pmcMetadataModel = new MetadataVideoModel(this);
    handleBusySignals(pmcMetadataModel);
    setModel(pmcMetadataModel);
}

void MetadataVideoBackend::search(const QString& searchTerm)
{
    qobject_cast<PmcMetadataModel*>(model())->setSearchTerm(searchTerm);
}

bool MetadataVideoBackend::supportsSearch() const
{
    return true;
}

#include "metadatavideobackend.moc"
