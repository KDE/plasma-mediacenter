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

#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QList>
#include <QWeakPointer>

#include <odb/core.hxx>

class Media;

//TODO: Album should also know its artist
#pragma db object
class Album
{
public:
    Album(const QString &name);
    ~Album();

    const QString& name() const;
    QString albumArtist() const;

private:
    Album();
    friend class odb::access;

    #pragma db id
    QString m_name;

    //See http://codesynthesis.com/products/odb/doc/manual.xhtml#6.2
    #pragma db inverse(m_album)
    QList< QWeakPointer<Media> > m_media;
};

#include "media.h"

#endif // ALBUM_H
