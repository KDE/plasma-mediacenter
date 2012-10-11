/*
 * This file is part of the KDE project
 *
 * Copyright (C) 2011 Shantanu Tushar <shantanu@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#ifndef PMCIMAGEPROVIDER_H
#define PMCIMAGEPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcImageProvider : public QDeclarativeImageProvider
{
public:
    static const char *identificationString;

    PmcImageProvider();
    virtual ~PmcImageProvider();
    virtual QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize);
};

#endif // PMCIMAGEPROVIDER_H
