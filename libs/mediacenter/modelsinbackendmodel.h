/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef MODELSINBACKENDMODEL_H
#define MODELSINBACKENDMODEL_H

#include <QAbstractListModel>

class PmcModel;

class ModelsInBackendModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        ModelRole = Qt::UserRole + 1
    };
    QVariant data(const QModelIndex& index, int role) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void addModel(PmcModel* model);
    bool replaceModel(PmcModel* original, PmcModel* replacement);
    void clear();

private:
    QList<PmcModel*> m_models;
};

#endif // MODELSINBACKENDMODEL_H
