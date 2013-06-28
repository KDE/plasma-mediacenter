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


#ifndef LOCALFILESABSTRACTBACKEND_H
#define LOCALFILESABSTRACTBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>
#include <KDE/KFilePlacesModel>
#include <Solid/Device>
#include <Solid/StorageAccess>

class LocalPlacesModel;   //forward declaration to tell the compiler that an instance will be declared
class LocalFilesAbstractModel;

class LocalFilesAbstractBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
public:
    LocalFilesAbstractBackend (QObject* parent, const QVariantList& args);

    virtual bool goOneLevelUp();
    virtual bool expand (int row);
    Q_INVOKABLE QObject *placesModel();
    bool browseToPlace(int row);
    Q_INVOKABLE void browseOneLevelUp();
    virtual bool okToLoad() const;

protected:
    virtual bool initImpl();
    virtual void initModel() = 0;
    LocalPlacesModel *m_placesModel;

private slots:
    void slotStorageSetupDone(Solid::ErrorType error, const QVariant& errordata, const QString& udi);

private:
    int m_placesRow;
    bool m_isShowingPlacesModel;
};

#endif // LOCALFILESABSTRACTBACKEND_H
