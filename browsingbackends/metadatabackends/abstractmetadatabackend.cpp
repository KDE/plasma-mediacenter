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
#include "abstractmetadatamodel.h"

#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeEngine>

AbstractMetadataBackend::AbstractMetadataBackend(QObject* parent, const QVariantList& args)
    : AbstractBrowsingBackend(parent, args)
{
}

void AbstractMetadataBackend::init()
{
    QDeclarativeComponent component(declarativeEngine());
    component.setData("import org.kde.metadatamodels 0.1\nMetadataModel {}\n", QUrl());

    setMetadataModel(component.create());
}

bool AbstractMetadataBackend::goOneLevelUp()
{
    return false;
}

bool AbstractMetadataBackend::okToLoad() const
{
    QDeclarativeComponent component(declarativeEngine());
    component.setData("import org.kde.metadatamodels 0.1\nMetadataModel {}\n", QUrl());

    return !component.isError();
}

#include "abstractmetadatabackend.moc"
