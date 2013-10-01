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


#ifndef MEDIAINFOREQUEST_H
#define MEDIAINFOREQUEST_H

#include <QtCore/QString>

class MediaInfoRequest
{
public:
    enum InformationField {
        Title,
        Artist,
        Length,
        Album
    };

    MediaInfoRequest(const QString &mediaPath);
    MediaInfoRequest *addRequest(InformationField field);
    bool hasFields() const;
    MediaInfoRequest::InformationField takeField();
    QString mediaPath() const;

private:
    class Private;
    Private * const d;
};

#endif // MEDIAINFOREQUEST_H
