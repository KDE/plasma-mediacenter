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

#include "mediainforesult.h"

#include "mediainforequest.h"

#include <QtCore/QList>
#include <QtCore/QHash>
#include <QtCore/QVariant>

class MediaInfoResult::Private
{
public:
    QHash<MediaInfoRequest::InformationField, QVariant> data;
};

MediaInfoResult::MediaInfoResult()
    : d(new Private())
{

}

MediaInfoResult::~MediaInfoResult()
{

}

void MediaInfoResult::addData(MediaInfoRequest::InformationField field, QVariant data)
{
    d->data.insert(field, data);
}

QList< MediaInfoRequest::InformationField > MediaInfoResult::availableFields() const
{
    return d->data.keys();
}

QVariant MediaInfoResult::data(MediaInfoRequest::InformationField field) const
{
    return d->data.value(field);
}
