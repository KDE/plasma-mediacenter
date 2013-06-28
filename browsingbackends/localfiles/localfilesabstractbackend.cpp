/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>
    Copyright (C) 2011  Shantanu Tushar <shantanu@kde.org>
    Copyright (C) 2013  Akshay Ratan  <akshayratan@gmail.com>

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
#include "localplacesmodel.h"

#include <KDE/KFilePlacesModel>
#include <KDE/KUrl>

LocalFilesAbstractBackend::LocalFilesAbstractBackend (QObject* parent, const QVariantList& args)
    : AbstractBrowsingBackend (parent, args)
    , m_placesModel(0)
    , m_placesRow(0)
{
    m_isShowingPlacesModel = false;
}

bool LocalFilesAbstractBackend::initImpl()
{
    setModel(qobject_cast<QAbstractItemModel*>(placesModel()));
    m_isShowingPlacesModel = true;
    return true;
}

void LocalFilesAbstractBackend::browseOneLevelUp()
{
    qobject_cast<LocalFilesAbstractModel*>(model())->goOneLevelUp();
}

bool LocalFilesAbstractBackend::goOneLevelUp()
{
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());

    if(m_isShowingPlacesModel)
    {
        m_isShowingPlacesModel=false;
        initModel();
        return qobject_cast<LocalFilesAbstractModel*>(model())->goOneLevelUp();
    }

    return filesModel->goOneLevelUp();
}

bool LocalFilesAbstractBackend::expand (int row)
{
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());
    QModelIndex m_index = m_placesModel->index(row,0);
    KUrl url =  m_placesModel->url(m_index);

    if(m_isShowingPlacesModel)
    {
        m_isShowingPlacesModel=false;
        initModel();
        return (qobject_cast<LocalFilesAbstractModel*>(model()))->browseToUrl(url);
    }

    return filesModel->browseTo(row);
}

QObject* LocalFilesAbstractBackend::placesModel()
{
    if (!m_placesModel) {
        m_placesModel = new LocalPlacesModel(this);
    }
    return m_placesModel;
}

void LocalFilesAbstractBackend::browseToPlace(int row)
{
    Solid::Device device;
    if (m_placesModel->isDevice(m_placesModel->index(row,0)))
    {
        device = m_placesModel->deviceForIndex(m_placesModel->index(row,0));
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
    KUrl url =  m_placesModel->url(m_placesModel->index(placeRow, 0));
    LocalFilesAbstractModel *filesModel = qobject_cast<LocalFilesAbstractModel*>(model());
    filesModel->browseToUrl(url);
}
#include "localfilesabstractbackend.h"
