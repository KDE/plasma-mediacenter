/*
    Copyright (C) 2013 Akshay Ratan <akshayratan@gmail.com>
    
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


#ifndef COLLECTIONSBACKEND_H
#define COLLECTIONSBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>

class CollectionsModel;  //Forward Declaration to use CollectionsModel

class CollectionsBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
public:
    explicit CollectionsBackend(QObject* parent, const QVariantList& args = QVariantList());
    virtual QString mediaBrowserOverride() const;
    virtual bool initImpl();
    Q_INVOKABLE QObject* collectionsModel();
  
private:
    CollectionsModel* m_collectionsModel;   
  
};

#endif // COLLECTIONSBACKEND_H
