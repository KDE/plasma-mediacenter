/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "mediacenter.h"

#include <QFileInfo>

#include <KMimeType>
#include <KUrl>

#include <Solid/Device>
#include <Solid/OpticalDisc>

#include <Phonon/MediaSource>

namespace MediaCenter {

QHash<int, QByteArray> appendAdditionalMediaRoles (const QHash<int, QByteArray> &roles)
{
    QHash<int, QByteArray> newRoles(roles);
    newRoles[MediaUrlRole] = "mediaUrl";
    newRoles[IsExpandableRole] = "isExpandable";
    newRoles[MediaTypeRole] = "mediaType";
    newRoles[DecorationTypeRole] = "decorationType";
    newRoles[HideLabelRole] = "hideLabel";
    newRoles[ResourceIdRole] = "resourceId";
    return newRoles;
}

} // MediaCenter namespace
