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

class LocalFilesAbstractBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
public:
    LocalFilesAbstractBackend (QObject* parent, const QVariantList& args);

    virtual void init();
    virtual bool goOneLevelUp();
    virtual bool expand (int row);
    virtual QString mediaBrowserSidePanel() const;
    Q_INVOKABLE QObject *placesModel();
    Q_INVOKABLE void browseToPlace(int row);

protected:
    virtual void initModel() = 0;
    KFilePlacesModel *m_placeModel;
};

#endif // LOCALFILESABSTRACTBACKEND_H
