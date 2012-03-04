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

#ifndef ABSTRACTMETADATAMODEL_H
#define ABSTRACTMETADATAMODEL_H

#include <QAbstractItemModel>

#include <Plasma/DataEngine>

class AbstractMetadataModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractMetadataModel (QObject* parent = 0);
    virtual ~AbstractMetadataModel ();

    virtual QAbstractItemModel* sourceModel() const;
    virtual void setSourceModel (QAbstractItemModel* sourceModel);
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount (const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent (const QModelIndex& child) const;
    virtual QModelIndex index (int row, int column, const QModelIndex& parent = QModelIndex()) const;

private slots:
    void sourceColumnsAboutToBeInserted (const QModelIndex& parent, int start, int end);
    void sourceColumnsAboutToBeMoved (const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn);
    void sourceColumnsAboutToBeRemoved (const QModelIndex& parent, int start, int end);
    void sourceColumnsInserted (const QModelIndex& parent, int start, int end);
    void sourceColumnsMoved (const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationColumn);
    void sourceColumnsRemoved (const QModelIndex& parent, int start, int end);
    void sourceDataChanged (const QModelIndex& topLeft, const QModelIndex& bottomRight);
    void sourceHeaderDataChanged (Qt::Orientation orientation, int first, int last);
    void sourceLayoutAboutToBeChanged ();
    void sourceLayoutChanged ();
    void sourceModelAboutToBeReset ();
    void sourceModelReset ();
    void sourceRowsAboutToBeInserted (const QModelIndex& parent, int start, int end);
    void sourceRowsAboutToBeMoved (const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow);
    void sourceRowsAboutToBeRemoved (const QModelIndex& parent, int start, int end);
    void sourceRowsInserted (const QModelIndex& parent, int start, int end);
    void sourceRowsMoved (const QModelIndex& sourceParent, int sourceStart, int sourceEnd, const QModelIndex& destinationParent, int destinationRow);
    void sourceRowsRemoved (const QModelIndex& parent, int start, int end);

protected:
    QAbstractItemModel* metadataModel() const;
    void resetMetadataModel();

private:
    class Private;
    Private* const d;
};

#endif // ABSTRACTMETADATAMODEL_H
