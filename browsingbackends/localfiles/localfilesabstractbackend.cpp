/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>
    Copyright (C) 2011  Shantanu Tushar <shantanu@kde.org>

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
    , m_placeModel(0)
    , m_placesRow(0)
{

}

bool LocalFilesAbstractBackend::initImpl()
{
    initModel();
    return true;
}

void LocalFilesAbstractBackend::browseOneLevelUp()
{
    qobject_cast<LocalFilesAbstractModel*>(model())->goOneLevelUp();
}

bool LocalFilesAbstractBackend::goOneLevelUp()
{
    return false;
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
    if (!m_placeModel) {
        m_placeModel = new KFilePlacesModel(this);
    }
    return m_placeModel;
}

void LocalFilesAbstractBackend::browseToPlace(int row)
{
    Solid::Device device;
    if (m_placeModel->isDevice(m_placeModel->index(row,0)))
    {
        device = m_placeModel->deviceForIndex(m_placeModel->index(row,0));
        const bool setup = !device.as<Solid::StorageAccess>()->isAccessible();
        if (setup) {
            m_placesRow = row;
            Solid::StorageAccess *access = device.as<Solid::StorageAccess>();
            connect(access, SIGNAL(setupDone(Solid::ErrorType,QVariant,QString)), this, SLOT(slotStorageSetupDone(Solid::ErrorType,QVariant,QString)));
            access->setup();
        } else {
            loadUrl(row);
        }
    } else {
        loadUrl(row);
    }
}

bool LocalFilesAbstractBackend::okToLoad() const
{
    KConfigGroup cg(KSharedConfig::openConfig("plasmamediacenterrc"), "Backends");
    return !cg.readEntry("hideLocalBrowsing", false);
}

void LocalFilesAbstractBackend::slotStorageSetupDone(Solid::ErrorType error,const QVariant& errorData,const QString& udi)
{
    loadUrl(m_placesRow);
}

void LocalFilesAbstractBackend::loadUrl(int placeRow)
{
    KUrl url =  m_placeModel->url(m_placeModel->index(placeRow, 0));
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());
    filesModel->browseToUrl(url);
}
#include "localfilesabstractbackend.h"
