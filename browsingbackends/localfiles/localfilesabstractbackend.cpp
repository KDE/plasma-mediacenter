/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>
    Copyright (C) 2011  Shantanu Tushar <shaan7in@gmail.com>

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


#include "localfilesabstractbackend.h"
#include "localfilesabstractmodel.h"

LocalFilesAbstractBackend::LocalFilesAbstractBackend (QObject* parent, const QVariantList& args)
    : AbstractBrowsingBackend (parent, args)
{

}

void LocalFilesAbstractBackend::init()
{
    initModel();
}

bool LocalFilesAbstractBackend::goOneLevelUp()
{
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());

    return filesModel->goOneLevelUp();
}

bool LocalFilesAbstractBackend::expand (int row)
{
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());

    return filesModel->browseTo(row);
}

QString LocalFilesAbstractBackend::mediaBrowserSidePanel() const
{
    return constructQmlSource("localfilescomponents", "0.1", "LocalFilesSidePanel");
}


QObject* LocalFilesAbstractBackend::placesModel()
{
     m_placeModel = new KFilePlacesModel(this);
     return m_placeModel;
}

void LocalFilesAbstractBackend::browseToPlace(int row)
{
    KUrl url =  m_placeModel->url(m_placeModel->index(row, 0));
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());
    filesModel->browseToUrl(url);
}

#include "localfilesabstractbackend.h"
