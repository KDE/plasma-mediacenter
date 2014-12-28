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

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>

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
    newRoles[DurationRole] = "mediaDuration";
    newRoles[ArtistRole] = "mediaArtist";
    newRoles[AlbumRole] = "mediaAlbum";
    newRoles[GenreRole] = "mediaGenre";
    newRoles[FallBackDecorationRole] = "fallbackDecoration";
    return newRoles;
}

QString dataDirForComponent(const QString& component)
{
  static const QString pmcPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
  if(!QDir(pmcPath).exists()) {
    QDir().mkpath(pmcPath);
  }
  return pmcPath + (component.isEmpty() ? QString() : QString("/%1").arg(component));
}

} // MediaCenter namespace
