/*
    Copyright (C) 2013 Akshay Ratan  <akshayratan@gmail.com>
    Copyright (c) 2007-2010 Sebastian Trueg <trueg@kde.org>
    Copyright (c) 2012-2013 Vishesh Handa <me@vhanda.in>
    
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

#include "collectionsmodel.h"

#include <QAbstractListModel>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QStringList>

namespace {
    QStringList defaultFolders() {
        return QStringList() << QDir::homePath();

    }
}

namespace {
    QStringList filterNonExistingDirectories(const QStringList& list) {
        QStringList finalList;
        foreach(const QString& path, list) {
            if( QFile::exists(path) )
                finalList << path;
        }
        return finalList;
    }
}

CollectionsModel::CollectionsModel( QObject* parent = 0)
    : QAbstractListModel( parent ) , pathList(0)
{
    
}

CollectionsModel::~CollectionsModel()
{
  
}
    /* This function would return the number of paths which are indexed by Nepomuk 
	in form of number of rows of the model */

int CollectionsModel::rowCount(const QModelIndex &parent) const
{
    return pathList.count();	
}

QVariant CollectionsModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }
    
    if (index.row() >= pathList.size()) {
        return QVariant();
    }
    
    if (role == Qt::DisplayRole) { 
        return pathList.at(index.row());
    }
     
    else {
      QFileInfo f;
      const QString path = f.filePath();
      return path;
    }
}

bool CollectionsModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if( index.isValid()) {
	QFileInfo f;
        const QString path = f.filePath();
        includePath( path );
        return true;
    }

    return QAbstractListModel::setData( index, value, role );
}

void CollectionsModel::includePath( const QString& path )
{
    pathList.append( path );
}

void CollectionsModel:: load()
{
    //File Indexer Settings
    KConfig fileIndexerConfig( "nepomukstrigirc" );
    KConfigGroup group = fileIndexerConfig.group("General");
      
    QStringList includeFolders = group.readPathEntry( "folders", defaultFolders());
    QStringList includes = filterNonExistingDirectories( includeFolders );
    setFolders(includes);
}

void CollectionsModel::setFolders( const QStringList& includeDirs)
{
     
     pathList = includeDirs;
     //This removes the duplicate entries from the pathList
     int m_removedEntries = pathList.removeDuplicates();  
}


#include "collectionsmodel.moc"
