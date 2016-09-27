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

#ifndef SINGLETONFACTORY_H
#define SINGLETONFACTORY_H

#include <QObject>
#include <QHash>

#include <typeinfo>

#include "kmediacollection_export.h"

class KMEDIACOLLECTION_EXPORT SingletonFactory
{
public:
    template <class T> static T* instanceFor()
    {
        const QString typeName(typeid(T).name());

        if (!instances.contains(typeName)) {
            instances.insert(typeName, static_cast<QObject*>(new T()));
        }

        return static_cast<T*>(instances.value(typeName));
    }
private:
    static QHash<QString, QObject*> instances;
};

#endif // SINGLETONFACTORY_H
