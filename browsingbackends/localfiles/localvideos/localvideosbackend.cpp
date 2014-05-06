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
#include "localvideosbackend.h"
#include "localvideosmodel.h"

#include "../localthumbnailprovider.h"

#include <mediacenter/pmcruntime.h>

MEDIACENTER_EXPORT_BROWSINGBACKEND(LocalVideosBackend)

LocalVideosBackend::LocalVideosBackend (QObject* parent, const QVariantList& args)
    : LocalFilesAbstractBackend (parent, args)
    , m_model(0)
{

}

LocalVideosBackend::~LocalVideosBackend()
{}

void LocalVideosBackend::initModel()
{
    if (!m_model) {
        ThumbnailProvider *thumbnailProvider = new ThumbnailProvider(this);
        pmcRuntime()->addImageProvider("localvideothumbnail", thumbnailProvider);

        m_model = new LocalVideosModel(thumbnailProvider, this);
    }
    setModel(m_model);
}

