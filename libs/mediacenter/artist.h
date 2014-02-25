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

#ifndef ARTIST_H
#define ARTIST_H

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "precompiled.h"

class Media;

class Artist
{
public:
    Artist();
    Artist(const QString &name);
    virtual ~Artist();

    QString m_name;
    const QString& name() const;

    QList< QSharedPointer<Media> > m_media;

    typedef QSharedPointer<Artist> Ptr;
    typedef QList<Ptr> List;
};

QX_REGISTER_PRIMARY_KEY(Artist, QString)
QX_REGISTER_HPP_PMC(Artist, qx::trait::no_base_class_defined, 0)

#endif // ARTIST_H
