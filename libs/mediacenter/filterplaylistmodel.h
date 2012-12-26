/***************************************************************************
 *   Copyright 2012 by Sujith Haridasan <sharidasan@kdemail.net>           *
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

#ifndef FILTERPLAYLISTMODEL_H
#define FILTERPLAYLISTMODEL_H

#include <QtGui/QSortFilterProxyModel>
#include <QAbstractItemModel>
#include "mediacenter_export.h"

class MEDIACENTER_EXPORT FilterPlaylistModel: public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString filterString READ filterString WRITE setFilterString NOTIFY filterStringChanged)
    Q_PROPERTY(QObject* sourcePlaylistModel READ sourcePlaylistModel WRITE setSourcePlaylistModel NOTIFY sourcePlaylistModelChanged)
    //Q_PROPERTY(QString filterString READ getFilter WRITE setFilter NOTIFY filterDataChanged)

public:
    FilterPlaylistModel(QObject* parent = 0);
    void setFilterString(const QString &customString);
    QString filterString() const;
    QObject *sourcePlaylistModel();
    void setSourcePlaylistModel(QObject *model);

signals:
    void filterStringChanged();
    void sourcePlaylistModelChanged();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString m_searchString;
};

#endif //FILTERPLAYLISTMODEL_H