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


#ifndef PMCARTIST_H
#define PMCARTIST_H

#include "precompiled.h"
#include "artist.h"

#include <QObject>
#include <QSharedPointer>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcArtist : public QObject
{
    Q_OBJECT
public:
    explicit PmcArtist(const QSharedPointer< Artist >& artist, QObject* parent = 0);
    virtual ~PmcArtist();

    const QString& name() const;

private:
    class Private;
    Private * const d;
};


#endif // PMCARTIST_H
