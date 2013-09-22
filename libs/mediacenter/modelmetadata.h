/***********************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef MEDIACENTER_MODELMETADATA_H
#define MEDIACENTER_MODELMETADATA_H

#include <QObject>
#include <QMetaType>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT ModelMetadata : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
public:
    explicit ModelMetadata(QObject* parent = 0);

    QString name() const;
    void setName(const QString &name);

signals:
    void nameChanged();

private:
    class Private;
    Private * const d;
};

#endif // MEDIACENTER_MODELMETADATA_H
