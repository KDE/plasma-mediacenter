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

#include "localfilesabstractmodel.h"

#include <KDirModel>
#include <KDirLister>

class LocalFilesAbstractModel::Private
{
public:
    KDirModel dirModel;
};

LocalFilesAbstractModel::LocalFilesAbstractModel (QObject* parent)
    : QAbstractItemModel (parent)
    , d(new Private())
{
    d->dirModel.dirLister()->openUrl(KUrl::fromLocalFile(QDir::homePath()));
}

int LocalFilesAbstractModel::columnCount (const QModelIndex& parent) const
{
    return d->dirModel.columnCount(parent);
}

int LocalFilesAbstractModel::rowCount (const QModelIndex& parent) const
{
    return d->dirModel.rowCount(parent);
}

QModelIndex LocalFilesAbstractModel::parent (const QModelIndex& child) const
{
    return d->dirModel.parent(child);
}

QModelIndex LocalFilesAbstractModel::index (int row, int column, const QModelIndex& parent) const
{
    return d->dirModel.index(row, column, parent);
}

KDirModel* LocalFilesAbstractModel::dirModel() const
{
    return &d->dirModel;
}

#include "localfilesabstractmodel.moc"
