/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  Shantanu Tushar <shantanu@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "iconimageprovider.h"

#include <QIcon>
#include <QDebug>

const char *IconImageProvider::identificationString = "icon";

IconImageProvider::IconImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap IconImageProvider::requestPixmap(const QString& id, QSize* size,
                                         const QSize& requestedSize)
{
    *size = requestedSize;
    return QIcon::fromTheme(id).pixmap(requestedSize);
}
