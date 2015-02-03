/***********************************************************************************
 *   Copyright 2015 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2015 Bhushan Shah    <bshah@kde.org>                                *
 *   Copyright 2015 Ashish Bansal   <bansal.ashish096@gmail.com>                   *
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

#ifndef UPNPMEDIASOURCE_H
#define UPNPMEDIASOURCE_H

#include <mediacenter/abstractmediasource.h>
#include <mediahandler.h>

class UPnPMediaSource : public MediaCenter::AbstractMediaSource
{
    Q_OBJECT
public:
    explicit UPnPMediaSource(QObject* parent = 0, const QVariantList& args = QVariantList());

protected:
    virtual void run();

private Q_SLOTS:
    void setupContextManager();

private:
    static void addMedia(QHash<int, QString> properties);
};

#endif // UPNPMEDIASOURCE_H
