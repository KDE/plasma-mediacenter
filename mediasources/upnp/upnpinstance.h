/*
 *  Copyright 2015 Ashish Bansal <bansal.ashish096@gmail.com>
 *  Copyright 2015 Bhushan Shah <bshah@kde.org>
 *  Copyright 2015 Shantanu Tushar <shantanu@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  2.010-1301, USA.
 */

#ifndef UPNPINSTANCE_H
#define UPNPINSTANCE_H

extern "C" {
    #include <libgupnp/gupnp-control-point.h>
    #include <libgupnp-av/gupnp-av.h>
    #include <libgssdp/gssdp.h>
}

struct BrowseData;

class UPnPInstance
{
public:
    UPnPInstance(GUPnPDeviceProxy *proxy);
    static void browseDirectChildren(GUPnPServiceProxy *contentDir, const char *containerId, guint32 startingIndex, guint32 requestedCount);
    static void browseDirectChildrenCallback(GUPnPServiceProxy *contentDir, GUPnPServiceProxyAction *action, gpointer userData);
    static void onDidlObjectAvailable(GUPnPDIDLLiteParser *parser, GUPnPDIDLLiteObject *object, gpointer userdata);
    static void appendDidlObject(GUPnPDIDLLiteObject *object, BrowseData *browseData, const char* udn);
    static void browseMetadata(GUPnPServiceProxy *contentDir, const char *id, const char *udn, gpointer userdata);
    static void browseMetadataCallback(GUPnPServiceProxy *contentDir, GUPnPServiceProxyAction *action, gpointer userdata);
    static GUPnPServiceProxy* getContentDir(GUPnPDeviceProxy *proxy);
};

#endif // UPNPINSTANCE_H
