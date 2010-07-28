/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#ifndef BACKENDMODEL_H
#define BACKENDMODEL_H

#include <QAbstractItemModel>
#include <KService>

namespace MediaCenter {
class AbstractBrowsingBackend;
}

class BackendModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    BackendModel(QObject *parent = 0);
    ~BackendModel();

    virtual int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
    virtual QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    Q_INVOKABLE virtual QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
    virtual QModelIndex parent ( const QModelIndex & index ) const;
    virtual int rowCount ( const QModelIndex & parent = QModelIndex() ) const;

    void setModelServices(const KService::List &services);

private:
    KService::List m_modelServices;
};

#endif // BACKENDMODEL_H
