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


#include "metadatapicturemodel.h"
#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <QtGui/QIcon>
#include <QEvent>

MetadataPictureModel::MetadataPictureModel (QObject* parent)
    : PmcMetadataModel (parent)
{
    showMediaType(MediaCenter::Picture);
}

MetadataPictureModel::~MetadataPictureModel()
{

}

QVariant MetadataPictureModel::data (const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DecorationRole:
        return PmcMetadataModel::data (index, MediaCenter::MediaUrlRole);
    case MediaCenter::MediaTypeRole:
        return "image";
    case MediaCenter::HideLabelRole:
        return true;
    }

    return PmcMetadataModel::data(index, role);
}

#include "metadatapicturemodel.moc"
