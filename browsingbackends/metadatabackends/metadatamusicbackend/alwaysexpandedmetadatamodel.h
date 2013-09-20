/***************************************************************************
 *   Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                  *
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

#ifndef ALWAYSEXPANDEDMETADATAMODEL_H
#define ALWAYSEXPANDEDMETADATAMODEL_H

#include <libs/mediacenter/pmcmetadatamodel.h>

class AlwaysExpandedMetadataModel : public PmcMetadataModel
{
    Q_OBJECT
public:
    explicit AlwaysExpandedMetadataModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif // ALWAYSEXPANDEDMETADATAMODEL_H
