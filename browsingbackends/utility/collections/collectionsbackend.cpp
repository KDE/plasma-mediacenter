/*
    Copyright (C) 2013 Akshay Ratan  <akshayratan@gmail.com>
    
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

#include "collectionsbackend.h"
#include <libs/mediacenter/abstractbrowsingbackend.h>
#include "collectionsmodel.h"

#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtCore/QMetaObject>
#include <QtCore/QDir>


MEDIACENTER_EXPORT_BROWSINGBACKEND(CollectionsBackend)

CollectionsBackend::CollectionsBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args) , m_collectionsModel(0)
{
  
}

QString CollectionsBackend::mediaBrowserOverride() const
{
    return constructQmlSource("add", "0.1", "AddMedia");
    
}

bool CollectionsBackend::initImpl()
{
    setModel(qobject_cast<QAbstractListModel*>(collectionsModel()));
    return true; 
}

QObject* CollectionsBackend::collectionsModel()
{
    m_collectionsModel = new CollectionsModel(this);
    m_collectionsModel->load();
    return m_collectionsModel;
}


