/*
    Copyright (C) 2011  Shantanu Tushar <shaan7in@gmail.com>

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

#include <KDirModel>

class LocalFilesAbstractModel : public KDirModel
{
    Q_OBJECT
public:
    explicit LocalFilesAbstractModel(QObject *parent, const QString &acceptedMimetypes);

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual bool goOneLevelUp();
    virtual bool browseTo(int row);

private:
    class Private;
    Private * const d;
};

#endif // LOCALFILESABSTRACTMODEL_H
