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


#include "metadatamusicmodel.h"
#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <QtGui/QIcon>
#include <QtCore/QEvent>
#include <QtDeclarative/QDeclarativePropertyMap>

#include <QDebug>

MetadataMusicModel::MetadataMusicModel (QObject* parent)
    : AbstractMetadataModel (parent)
{
}

MetadataMusicModel::~MetadataMusicModel()
{

}

void MetadataMusicModel::setAlbumName(const QString& albumName)
{
    m_albumName = albumName;
}

void MetadataMusicModel::setArtistName(const QString& artistName)
{
    m_artistName = artistName;
}

void MetadataMusicModel::updateModel()
{
    QDeclarativePropertyMap *map
            = qobject_cast<QDeclarativePropertyMap*>(metadataModel()->property("extraParameters").value<QObject*>());

    resetMetadataModel();
    map->insert("nmm:musicAlbum", m_albumName);
    map->insert("nmm:performer", m_artistName);

    metadataModel()->setProperty("resourceType", "nfo:Audio");
}

QVariant MetadataMusicModel::data (const QModelIndex& index, int role) const
{
    if (!metadataModel()) {
        return QVariant();
    }

    if(role == MediaCenter::MediaTypeRole) {
        return "audio";
    }
    else {
        return AbstractMetadataModel::data(index, role);
    }
}

#include "metadatamusicmodel.moc"
