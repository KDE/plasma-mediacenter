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


#include "metadatamusicbackend.h"

#include "metadatamusicmodel.h"

#include <QDebug>
#include "categoriesmodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend)

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
{
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

void MetadataMusicBackend::init()
{
    if (metadataModel()) {
        setModel(new CategoriesModel(this));
    }
    AbstractMetadataBackend::init();
}

bool MetadataMusicBackend::expand(int row)
{
    MetadataMusicModel *filesModel = qobject_cast<MetadataMusicModel*>(model());

    return filesModel->browseTo(row);
}

bool MetadataMusicBackend::goOneLevelUp()
{
     MetadataMusicModel *filesModel = qobject_cast<MetadataMusicModel*>(model());
    return filesModel->goOneLevelUp();
}

#include "metadatamusicbackend.moc"
