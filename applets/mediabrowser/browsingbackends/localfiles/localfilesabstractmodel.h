/*
    Copyright (C) 2011  Shantanu Tushar <jhahoneyk@gmail.com>

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

#include <QtCore/QAbstractItemModel>

class LocalFilesAbstractModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit LocalFilesAbstractModel(QObject *parent);

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex& child) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual bool goOneLevelUp();
    virtual bool browseTo(int row);

private:
    class Private;
    Private * const d;
};

#endif // LOCALFILESABSTRACTMODEL_H
