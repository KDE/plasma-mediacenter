/*
    Copyright (C) 2013  Akshay Ratan akshayratan@gmail.com

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

#include "localplacesmodel.h"

#include <kfileplacesmodel.h>
#include <kfileitem.h>

#include <mediacenter/mediacenter.h>
#include <KDE/KFilePlacesModel>


class LocalPlacesModel::Private
{
public:
    KFilePlacesModel placesModel;
};

LocalPlacesModel::LocalPlacesModel(QObject* parent) : KFilePlacesModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

QVariant LocalPlacesModel:: data(const QModelIndex& index , int role) const
{
    switch (role) {
        case MediaCenter::IsExpandableRole:
            return true;
        case MediaCenter::MediaUrlRole:
            return data(index, KFilePlacesModel::UrlRole).value<KFileItem>().url().prettyUrl();
    }

    return KFilePlacesModel::data (index, role);
}

