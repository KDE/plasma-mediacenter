/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>
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

#ifndef METADATAPICTUREBACKEND_H
#define METADATAPICTUREBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>

#include "../abstractmetadatabackend.h"

class MetadataPictureBackend : public AbstractMetadataBackend
{
    Q_OBJECT
public:
    MetadataPictureBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataPictureBackend();
    virtual QString backendCategory() const;

public Q_SLOTS:
    virtual void init();
};

#endif // METADATAPICTUREBACKEND_H
