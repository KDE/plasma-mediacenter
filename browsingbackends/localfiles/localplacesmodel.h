/*
    Copyright (C) 2013 Akshay Ratan <akshayratan@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/


#ifndef LOCALPLACESMODEL_H
#define LOCALPLACESMODEL_H

#include <KDE/KFilePlacesModel>

/**
  * @class LocalPlacesModel @author Akshay Ratan
  * @brief Model for LocalPlacesModel
  *
  * @description  This class contains all the properties of the Local Places Browsing.
  * 
  * This class inherits from KFilePlacesModel which is a list view model.
  */

class LocalPlacesModel : public KFilePlacesModel
{
    Q_OBJECT
public:
    
     /**
     * Constructor
     * @brief Used for initialization purposes.
     * @param parent is a QObject pointer
     */
    explicit LocalPlacesModel(QObject *parent);

     /**
     * Returns the data stored under the given role for the item referred to by the index of the model.
     * @param index is of QModelIndex Type
     * @param role is set to Qt:: DisplayRole which means the key data is to be rendered in the form of text
     */
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;

private:
    class Private;
};

#endif // LOCALPLACESMODEL_H
