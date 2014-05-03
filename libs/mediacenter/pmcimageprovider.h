/***********************************************************************************
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef PMCIMAGEPROVIDER_H
#define PMCIMAGEPROVIDER_H

#include <QtQuick/QQuickImageProvider>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcImageProvider : public QQuickImageProvider
{
public:
    static const char *identificationString;

    PmcImageProvider();
    virtual ~PmcImageProvider();
    virtual QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
};

#endif // PMCIMAGEPROVIDER_H
