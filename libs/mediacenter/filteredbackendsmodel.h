/***************************************************************************
 *   Copyright 2012 by Shantanu Tushar <shantanu@kde.org>                  *
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

#ifndef FILTEREDBACKENDSMODEL_H
#define FILTEREDBACKENDSMODEL_H

#include <QtGui/QSortFilterProxyModel>
#include "mediacenter_export.h"

class MEDIACENTER_EXPORT FilteredBackendsModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString backendCategory READ backendCategory WRITE setBackendCategory NOTIFY backendCategoryChanged)
    Q_PROPERTY(QObject* sourceBackendsModel READ sourceBackendsModel WRITE setSourceBackendsModel NOTIFY sourceBackendsModelChanged)

public:
    FilteredBackendsModel(QObject* parent = 0);
    virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

    QString backendCategory() const;
    void setBackendCategory(const QString &category);
    QObject *sourceBackendsModel();
    void setSourceBackendsModel(QObject *model);

signals:
    void backendCategoryChanged();
    void sourceBackendsModelChanged();

private:
    QString m_category;
};

#endif // FILTEREDBACKENDSMODEL_H
