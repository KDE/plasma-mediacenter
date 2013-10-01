/*
    Copyright (C) 2011 Shantanu Tushar <shantanu@kde.org>
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


#ifndef LOCALFILESABSTRACTMODEL_H
#define LOCALFILESABSTRACTMODEL_H

#include <KDE/KDirSortFilterProxyModel>
#include <KDE/KUrl>

  /**
  * @class LocalFilesAbstractModel @author Shantanu Tushar @author Akshay Ratan
  * @brief Model for Local File Browsing
  *
  * @description  This class contains all the properties and logic of the Local File Browsing.
  * Functions to go back one level up and browsing to a particular file URL are implemented in this.
  * 
  * This class inherits from KDirSortFilterProxyModel which acts as a proxy model
  * for KDirModel to sort and filter KFileItems
  */
class LocalFilesAbstractModel : public KDirSortFilterProxyModel
{
    Q_OBJECT
public:
    
   /**
     * Constructor
     * @brief Used for initialization purposes.
     * @param parent is a QObject pointer
     * @param acceptedMimetypes which is of QString type
     */
    explicit LocalFilesAbstractModel(QObject *parent, const QString &acceptedMimetypes);

    /**
     * Returns the data stored under the given role for the item referred to by the index.
     * @param index is of QModelIndex Type
     * @param role is set to Qt:: DisplayRole which means the key data is to be rendered in the form of text
     */
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    
    /**
     * Used to go one level back/up the current mediabrowser page. Stack is implemented
     * to store the file url history
     */
    virtual bool goOneLevelUp();
    
    /**
     * Calls browseToUrl function
     * Also uses addPath function which adds the url to the currentPath
     * @param row is the row number of the model 
     */
    virtual bool browseTo(int row);
    
    /**
     * Browses to the url passed
     * @param url is of KUrl type which is an address of the current file user wants to browse
     */
    
    virtual bool browseToUrl(const KUrl& url);

private:
    class Private;
    Private * const d;
};

#endif // LOCALFILESABSTRACTMODEL_H
