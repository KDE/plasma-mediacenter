/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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


#include "metadatamusicmodel.h"
#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <KDebug>

#include <QtGui/QIcon>
#include <QEvent>

class MetadataMusicModel::Private
{
public:
    Private() {
        model = 0;
    }

    QAbstractItemModel* model;
};

MetadataMusicModel::MetadataMusicModel (QObject* parent)
    : QAbstractItemModel (parent)
    , d (new Private())
{
    if (metadataModel()) {
        setSourceModel(metadataModel());
        metadataModel()->setProperty("resourceType", "nfo:Audio");
    }
    else {
        kDebug() << "WARNING: Constructor called before metadataModel set :/";
    }
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

MetadataMusicModel::~MetadataMusicModel()
{
    delete d;
}

QAbstractItemModel* MetadataMusicModel::sourceModel() const
{
    return d->model;
}

void MetadataMusicModel::setSourceModel (QAbstractItemModel* newSourceModel)
{
    if (sourceModel()) {
        disconnect (sourceModel(), SIGNAL (columnsAboutToBeInserted (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (columnsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)));
        disconnect (sourceModel(), SIGNAL (columnsAboutToBeRemoved (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (columnsInserted (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (columnsMoved (QModelIndex, int, int, QModelIndex, int)));
        disconnect (sourceModel(), SIGNAL (columnsRemoved (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (dataChanged (QModelIndex, QModelIndex)));
        disconnect (sourceModel(), SIGNAL (headerDataChanged (Qt::Orientation, int, int)));
        disconnect (sourceModel(), SIGNAL (layoutAboutToBeChanged ()));        disconnect (sourceModel(), SIGNAL (layoutChanged ()));        disconnect (sourceModel(), SIGNAL (modelAboutToBeReset()));        disconnect (sourceModel(), SIGNAL (modelReset ()));        disconnect (sourceModel(), SIGNAL (rowsAboutToBeInserted (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (rowsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)));
        disconnect (sourceModel(), SIGNAL (rowsAboutToBeRemoved (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (rowsInserted (QModelIndex, int, int)));
        disconnect (sourceModel(), SIGNAL (rowsMoved (QModelIndex, int, int, QModelIndex, int)));
        disconnect (sourceModel(), SIGNAL (rowsRemoved (QModelIndex, int, int)));
    }

    d->model = newSourceModel;

    if (sourceModel()) {
        connect (sourceModel(), SIGNAL (columnsAboutToBeInserted (QModelIndex, int, int)),
                 SLOT (sourceColumnsAboutToBeInserted (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (columnsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)),
                 SLOT (sourceColumnsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)));
        connect (sourceModel(), SIGNAL (columnsAboutToBeRemoved (QModelIndex, int, int)),
                 SLOT (sourceColumnsAboutToBeRemoved (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (columnsInserted (QModelIndex, int, int)),
                 SLOT (sourceColumnsInserted (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (columnsMoved (QModelIndex, int, int, QModelIndex, int)),
                 SLOT (sourceColumnsMoved (QModelIndex, int, int, QModelIndex, int)));
        connect (sourceModel(), SIGNAL (columnsRemoved (QModelIndex, int, int)),
                 SLOT (sourceColumnsRemoved (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (dataChanged (QModelIndex, QModelIndex)),
                 SLOT (sourceDataChanged (QModelIndex, QModelIndex)));
        connect (sourceModel(), SIGNAL (headerDataChanged (Qt::Orientation, int, int)),
                 SLOT (sourceHeaderDataChanged (Qt::Orientation, int, int)));
        connect (sourceModel(), SIGNAL (layoutAboutToBeChanged ()), SLOT (sourceLayoutAboutToBeChanged ()));
        connect (sourceModel(), SIGNAL (layoutChanged ()), SLOT (sourceLayoutChanged ()));
        connect (sourceModel(), SIGNAL (modelAboutToBeReset()), SLOT (sourceModelAboutToBeReset ()));
        connect (sourceModel(), SIGNAL (modelReset ()), SLOT (sourceModelReset ()));
        connect (sourceModel(), SIGNAL (rowsAboutToBeInserted (QModelIndex, int, int)),
                 SLOT (sourceRowsAboutToBeInserted (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (rowsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)),
                 SLOT (sourceRowsAboutToBeMoved (QModelIndex, int, int, QModelIndex, int)));
        connect (sourceModel(), SIGNAL (rowsAboutToBeRemoved (QModelIndex, int, int)),
                 SLOT (sourceRowsAboutToBeRemoved (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (rowsInserted (QModelIndex, int, int)),
                 SLOT (sourceRowsInserted (QModelIndex, int, int)));
        connect (sourceModel(), SIGNAL (rowsMoved (QModelIndex, int, int, QModelIndex, int)),
                 SLOT (sourceRowsMoved (QModelIndex, int, int, QModelIndex, int)));
        connect (sourceModel(), SIGNAL (rowsRemoved (QModelIndex, int, int)),
                 SLOT (sourceRowsRemoved (QModelIndex, int, int)));
    }
}

QVariant MetadataMusicModel::data (const QModelIndex& index, int role) const
{
    if (!metadataModel())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return metadataModel()->data(index, metadataModel()->roleNames().key("label"));
        break;
    case Qt::DecorationRole:
        return metadataModel()->data(index, metadataModel()->roleNames().key("icon"));
        break;
    case MediaCenter::MediaUrlRole:
        return metadataModel()->data(index, metadataModel()->roleNames().key("url"));
        break;
    case MediaCenter::IsExpandableRole:
        return false;
        break;
    case MediaCenter::MediaTypeRole:
        return "audio";
    }

    return QVariant();
}

int MetadataMusicModel::columnCount (const QModelIndex& parent) const
{
    return 1;
}

int MetadataMusicModel::rowCount (const QModelIndex& parent) const
{
    if (!metadataModel())
        return 0;
    return metadataModel()->rowCount(parent);
}

QModelIndex MetadataMusicModel::parent (const QModelIndex& child) const
{
    return QModelIndex();
}

QModelIndex MetadataMusicModel::index (int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return metadataModel()->index(row, column, parent);
}

QAbstractItemModel* MetadataMusicModel::metadataModel() const
{
    MediaCenter::AbstractBrowsingBackend *backend
        = static_cast<MediaCenter::AbstractBrowsingBackend*>(QObject::parent());
    return static_cast<QAbstractItemModel*>(backend->metadataModel());
}

void MetadataMusicModel::sourceColumnsAboutToBeInserted ( const QModelIndex & parent, int start, int end )
{
    beginInsertColumns(parent, start, end);
}

void MetadataMusicModel::sourceColumnsAboutToBeMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn )
{
    beginMoveColumns(sourceParent, sourceStart, sourceEnd, destinationParent, destinationColumn);
}

void MetadataMusicModel::sourceColumnsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
    beginRemoveColumns(parent, start, end);
}

void MetadataMusicModel::sourceColumnsInserted ( const QModelIndex & parent, int start, int end )
{
    insertColumns(start, end, parent);
}

void MetadataMusicModel::sourceColumnsMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationColumn )
{
    endMoveColumns();
}

void MetadataMusicModel::sourceColumnsRemoved ( const QModelIndex & parent, int start, int end )
{
    removeColumns(start, end, parent);
}

void MetadataMusicModel::sourceDataChanged ( const QModelIndex & topLeft, const QModelIndex & bottomRight )
{
    emit dataChanged(topLeft, bottomRight);
}

void MetadataMusicModel::sourceHeaderDataChanged ( Qt::Orientation orientation, int first, int last )
{
    emit headerDataChanged(orientation, first, last);
}

void MetadataMusicModel::sourceLayoutAboutToBeChanged ()
{
    emit layoutAboutToBeChanged();
}

void MetadataMusicModel::sourceLayoutChanged ()
{
    emit layoutChanged();
}

void MetadataMusicModel::sourceModelAboutToBeReset ()
{
    beginResetModel();
}

void MetadataMusicModel::sourceModelReset ()
{
    reset();
}

void MetadataMusicModel::sourceRowsAboutToBeInserted ( const QModelIndex & parent, int start, int end )
{
    beginInsertRows(parent, start, end);
}

void MetadataMusicModel::sourceRowsAboutToBeMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow )
{
    beginMoveRows(sourceParent, sourceStart, sourceEnd, destinationParent, destinationRow);
}

void MetadataMusicModel::sourceRowsAboutToBeRemoved ( const QModelIndex & parent, int start, int end )
{
    beginRemoveRows(parent, start, end);
}

void MetadataMusicModel::sourceRowsInserted ( const QModelIndex & parent, int start, int end )
{
    endInsertRows();
}

void MetadataMusicModel::sourceRowsMoved ( const QModelIndex & sourceParent, int sourceStart, int sourceEnd, const QModelIndex & destinationParent, int destinationRow )
{
    endMoveRows();
}

void MetadataMusicModel::sourceRowsRemoved ( const QModelIndex & parent, int start, int end )
{
    removeRows(start, end, parent);
}

#include "metadatamusicmodel.moc"
