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

#ifndef MEDIAINFOSERVICEPROXY_H
#define MEDIAINFOSERVICEPROXY_H

#include <QtCore/QPair>

class MediaInfoRequest;
class MediaInfoService;

class MediaInfoServiceProxy
{
public:
    static MediaInfoServiceProxy *instance();
    ~MediaInfoServiceProxy();

    QPair<quint64, MediaInfoService*> processRequest(MediaInfoRequest* request);

private:
    MediaInfoServiceProxy();
    static MediaInfoServiceProxy *m_instance;
    void incrementCounter();

    class Private;
    Private * const d;
};

#endif // MEDIAINFOSERVICEPROXY_H
