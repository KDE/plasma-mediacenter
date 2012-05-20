/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Shantanu Tushar shaan7in@gmail.com

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "localpicturesmodel.h"

#include <mediacenter/mediacenter.h>

LocalPicturesModel::LocalPicturesModel (QObject* parent) : LocalFilesAbstractModel (parent, QString("image/"))
{

}

QVariant LocalPicturesModel::data (const QModelIndex& index, int role) const
{
    if (role == Qt::DecorationRole) {
        if (!LocalPicturesModel::data(index, MediaCenter::IsExpandableRole).toBool())
            return LocalFilesAbstractModel::data (index, MediaCenter::MediaUrlRole);
    } else if(role == MediaCenter::HideLabelRole) {
        return !LocalPicturesModel::data(index, MediaCenter::IsExpandableRole).toBool();
    }
    return LocalFilesAbstractModel::data (index, role);
}
