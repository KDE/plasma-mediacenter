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

#ifndef ITEMCACHE_H
#define ITEMCACHE_H

#include <QSharedPointer>
#include <QHash>
#include <QVariant>

template <class T>
class ItemCache
{
public:
    QSharedPointer<T> getById(const QString &id, bool create = false)
    {
        if (create && !m_items.contains(id)) {
            m_items.insert(id, QSharedPointer<T>(new T(id)));
        }

        return m_items.value(id);
    }

private:
    QHash< QString, QSharedPointer<T> > m_items;
};

#endif // ITEMCACHE_H
