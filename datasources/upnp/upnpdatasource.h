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

#include "mediahandler.h"
#include <mediacenter/abstractdatasource.h>

#include <QStringList>

typedef struct _GUPnPDeviceProxy GUPnPDeviceProxy;
typedef struct _GUPnPControlPoint GUPnPControlPoint;
typedef struct _GUPnPContext GUPnPContext;
typedef struct _GUPnPContextManager GUPnPContextManager;
typedef void* gpointer;
class UPnPInstance;

class UPnPDataSource : public MediaCenter::AbstractDataSource
{
    Q_OBJECT
public:
    explicit UPnPDataSource(QObject* parent = 0, const QVariantList& args = QVariantList());
    static void addMedia(QHash<int, QString> properties);
    static QList< QPair< QString, QString > > mediaList;

private:
    static void onContextAvailable(GUPnPContextManager *contextManager, GUPnPContext *context, gpointer userData);
    static void mediaServerProxyAvailable(GUPnPControlPoint *cp, GUPnPDeviceProxy  *proxy);
    static void mediaServerProxyUnavailable(GUPnPControlPoint *cp, GUPnPDeviceProxy  *proxy);
    static void addMediaServer(GUPnPDeviceProxy *proxy);
    static void removeMediaServer(GUPnPDeviceProxy *proxy);
    static void rescanMediaServer(GUPnPDeviceProxy *proxy);
    static GUPnPContextManager *contextManager;
    static int port;
    static QList< QPair< QString, UPnPInstance* > > mediaservers;

protected:
    virtual void run();

private Q_SLOTS:
    static void setupContextManager();

};

#endif // UPNPMEDIASOURCE_H
