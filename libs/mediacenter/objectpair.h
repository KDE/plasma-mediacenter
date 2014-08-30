/***********************************************************************************
 *  Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                           *
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

#ifndef OBJECTPAIR_H
#define OBJECTPAIR_H

#include "mediacenter_export.h"

#include <QObject>
#include <QPair>

class MEDIACENTER_EXPORT ObjectPair : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* first READ first WRITE setFirst NOTIFY firstChanged)
    Q_PROPERTY(QObject* second READ second WRITE setSecond NOTIFY secondChanged)

public:
    explicit ObjectPair(QObject* parent = 0);
    QObject *first() const;
    void setFirst(QObject *object);

    QObject *second() const;
    void setSecond(QObject *object);

signals:
    void firstChanged();
    void secondChanged();

private:
    QPair<QObject*, QObject*> m_pair;
};

#endif // OBJECTPAIR_H
