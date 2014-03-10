/***************************************************************************
 *   Copyright 2014 by Shantanu Tushar <shantanu@kde.org>                  *
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

#include "metadatamusicsongsmodel.h"

MetadataMusicSongsModel::MetadataMusicSongsModel(QObject* parent): MetadataBackendCommonModel(parent)
{

}

MetadataMusicSongsModel::~MetadataMusicSongsModel()
{

}

QVariant MetadataMusicSongsModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        const QString songName = MetadataBackendCommonModel::data(index, role).toString();
        const QString artistName = MetadataBackendCommonModel::data(index, MediaCenter::ArtistRole).toString();
        const QString albumName = MetadataBackendCommonModel::data(index, MediaCenter::AlbumRole).toString();

        QVariantList list;
        list << songName << albumName << artistName;
        return list;
    }

    return MetadataBackendCommonModel::data(index, role).toString();
}
