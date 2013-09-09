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

#ifndef _FOLDER_SELECTION_MODEL_H_
#define _FOLDER_SELECTION_MODEL_H_

#include <QFileSystemModel>
#include <QtCore/QSet>
#include <QFileInfo>

#include <KCModule>
#include <KConfig>
#include <KConfigGroup>


class CollectionsModel : public QAbstractListModel
{
    Q_OBJECT

public:

    CollectionsModel(QObject* parent);
    virtual ~CollectionsModel();

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void includePath( const QString& path );
    virtual void load();
    void setFolders( const QStringList& includeDirs);

protected: 
    bool setData( const QModelIndex& index, const QVariant& value, int role );

private:
    QStringList pathList;
    
};

#endif
