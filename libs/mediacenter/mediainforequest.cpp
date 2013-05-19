/***********************************************************************************
 *   Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                          *
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


#include "mediainforequest.h"

#include <QtCore/QList>

class MediaInfoRequest::Private
{
public:
    QList<InformationField> requestedFields;
    QString mediaPath;
};

MediaInfoRequest::MediaInfoRequest(const QString& mediaPath)
    : d(new Private())
{
    d->mediaPath = mediaPath;
}

MediaInfoRequest* MediaInfoRequest::addRequest(MediaInfoRequest::InformationField field)
{
    d->requestedFields.append(field);
    return this;
}

bool MediaInfoRequest::hasFields() const
{
    return !d->requestedFields.isEmpty();
}

MediaInfoRequest::InformationField MediaInfoRequest::takeField()
{
    return d->requestedFields.takeFirst();
}

QString MediaInfoRequest::mediaPath() const
{
    return d->mediaPath;
}
