/***********************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>            *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "mediacenter.h"

#include <QFileInfo>

#include <KMimeType>
#include <KUrl>
#include <KCmdLineArgs>
#include <KStandardDirs>

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
    newRoles[MediaThumbnailRole] = "mediaThumbnail";
    newRoles[DurationRole] = "mediaDuration";
    newRoles[ArtistRole] = "mediaArtist";
    newRoles[AlbumRole] = "mediaAlbum";
    return newRoles;
}

QString dataDirForComponent(const QString& component)
{
    return KGlobal::dirs()->saveLocation("data")
                                    + KCmdLineArgs::appName()
                                    + QString("/%1/").arg(component);
}

} // MediaCenter namespace
