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
    KDirModel *model;
};

LocalFilesAbstractModel::LocalFilesAbstractModel (QObject* parent)
    : QAbstractItemModel (parent)
    , d(new Private())
{
    d->model = new KDirModel(this);
    d->model->dirLister()->openUrl(KUrl::fromLocalFile(QDir::homePath()));
}

int LocalFilesAbstractModel::columnCount (const QModelIndex& parent) const
{
    return 1;
}

QModelIndex LocalFilesAbstractModel::index (int row, int column, const QModelIndex& parent) const
{
    return d->model->index(row, column, parent);
}

QModelIndex LocalFilesAbstractModel::parent (const QModelIndex& child) const
{
    return d->model->parent(child);
}

int LocalFilesAbstractModel::rowCount (const QModelIndex& parent) const
{
    return d->model->rowCount(parent);
}

QVariant LocalFilesAbstractModel::data (const QModelIndex& index, int role) const
{
    return d->model->data(index, role);
}

bool LocalFilesAbstractModel::goOneLevelUp()
{
    KUrl url = d->model->dirLister()->url();

    if (QDir(url.toLocalFile()) == QDir(QDir::homePath())) {
        return false;
    }

    url.addPath("..");
    bool success = d->model->dirLister()->openUrl(url);

    return success;
}

bool LocalFilesAbstractModel::browseTo (int row)
{
    KUrl url = d->model->dirLister()->url();
    url.addPath(data(index(row, 0)).toString());
    bool success = d->model->dirLister()->openUrl(url);

    return success;
}

#include "localfilesabstractmodel.moc"
