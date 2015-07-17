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

#define MAX_BROWSE 64
#define CONTENT_DIR "urn:schemas-upnp-org:service:ContentDirectory"

#include "upnpinstance.h"
#include "upnpmediasource.h"
#include "mediacenter/mediacenter.h"
#include "mediacenter/abstractmediasource.h"

#include <QHash>
#include <QDebug>
#include <QString>
#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>

struct BrowseData
{
    GUPnPServiceProxy *contentDir;
    gchar *id;
    guint32 startingIndex;
};

struct BrowseMetadataData
{
    gchar *id;
    gpointer userdata;
    QString udn;
};

UPnPInstance::UPnPInstance(GUPnPDeviceProxy* proxy)
{
    GUPnPDeviceInfo *info;
    GUPnPServiceProxy *contentDir;
    QString friendlyName;

    info = GUPNP_DEVICE_INFO(proxy);
    contentDir = getContentDir(proxy);
    friendlyName = gupnp_device_info_get_friendly_name(info);

    qDebug() << "Friendly Name" << friendlyName;

    browseDirectChildren(contentDir, "0", 0, MAX_BROWSE);
    gupnp_service_proxy_set_subscribed(contentDir, TRUE);
}

void UPnPInstance::browseDirectChildren(GUPnPServiceProxy *contentDir, const char *containerId, guint32 startingIndex, guint32 requestedCount)
{
    BrowseData *data;
    data = g_slice_new(BrowseData);
    data->id = g_strdup(containerId);
    data->startingIndex = startingIndex;
    data->contentDir = (GUPnPServiceProxy*) g_object_ref(contentDir);

    qDebug() << "Container ID and Starting Index if item is container:" << containerId << startingIndex << endl;

    gupnp_service_proxy_begin_action(contentDir,
                                     "Browse",
                                     UPnPInstance::browseDirectChildrenCallback,
                                     data,
                                     /* IN args */
                                     "ObjectID",
                                     G_TYPE_STRING,
                                     containerId,
                                     "BrowseFlag",
                                     G_TYPE_STRING,
                                     "BrowseDirectChildren",
                                     "Filter",
                                     G_TYPE_STRING,
                                     "@childCount",
                                     "StartingIndex",
                                     G_TYPE_UINT,
                                     startingIndex,
                                     "RequestedCount",
                                     G_TYPE_UINT,
                                     requestedCount,
                                     "SortCriteria",
                                     G_TYPE_STRING,
                                     "",
                                     nullptr);
}

GUPnPServiceProxy* UPnPInstance::getContentDir(GUPnPDeviceProxy *proxy)
{
    GUPnPDeviceInfo *info;
    GUPnPServiceInfo *contentDir;
    info = GUPNP_DEVICE_INFO(proxy);
    contentDir = gupnp_device_info_get_service(info, CONTENT_DIR);
    return GUPNP_SERVICE_PROXY(contentDir);
}

void UPnPInstance::browseMetadataCallback(GUPnPServiceProxy *contentDir, GUPnPServiceProxyAction *action, gpointer userdata)
{
    GError *error = nullptr;
    char *metadata = nullptr;
    BrowseMetadataData *data = (BrowseMetadataData *) userdata;
    gupnp_service_proxy_end_action(contentDir,
                                   action,
                                   &error,
                                   /* OUT args */
                                   "Result",
                                   G_TYPE_STRING,
                                   &metadata,
                                   nullptr);
    if (metadata) {
        QString artist, album, albumArtist, title, mediaType, duration, url, protocolInfo, mimeType;
        QDomElement docElem;
        QDomDocument xmlDoc;
        xmlDoc.setContent(QByteArray(metadata));
        docElem = xmlDoc.documentElement();
        QDomNodeList itemList = docElem.elementsByTagName("item");
        for(int i = 0; i < itemList.count(); i++){
            QDomElement item = itemList.at(i).toElement();
            mimeType = item.elementsByTagName("upnp:class").at(0).toElement().text();

            if (mimeType.contains("audio", Qt::CaseInsensitive)) {
                mimeType = "audio";
            } else if (mimeType.contains("video", Qt::CaseInsensitive)) {
                mimeType = "video";
            } else if (mimeType.contains("image", Qt::CaseInsensitive)) {
                mimeType = "image";
            } else {
                continue;
            }

            url = item.elementsByTagName("res").at(0).toElement().text();
            title = item.elementsByTagName("dc:title").at(0).toElement().text();
            album = item.elementsByTagName("upnp:album").at(0).toElement().text();
            artist = item.elementsByTagName("upnp:artist").at(0).toElement().text();
            duration = item.elementsByTagName("res").at(0).toElement().attribute("duration");
            protocolInfo= item.elementsByTagName("res").at(0).toElement().attribute("protocolInfo");

            QHash<int, QString> properties;
            properties.insert(Qt::DisplayRole, title);
            properties.insert(MediaCenter::AlbumRole, album);
            properties.insert(MediaCenter::MediaUrlRole, url);
            properties.insert(MediaCenter::ArtistRole, artist);
            properties.insert(MediaCenter::MimeTypeRole, mimeType);
            properties.insert(MediaCenter::DurationRole, duration);
            properties.insert(MediaCenter::AlbumArtistRole, albumArtist);
            UPnPMediaSource::addMedia(properties);
        }
        g_free(metadata);
    } else if (error) {
        g_warning("Failed to get metadata for '%s': %s",
                   data->id,
                   error->message);

        g_error_free(error);
    }
}

void UPnPInstance::browseMetadata(GUPnPServiceProxy *contentDir, const char *id, const char *udn, gpointer userdata)
{
    BrowseMetadataData *data;
    data = g_slice_new(BrowseMetadataData);
    data->id = g_strdup(id);
    data->userdata = userdata;
    gupnp_service_proxy_begin_action((GUPnPServiceProxy*)g_object_ref(contentDir),
                                     "Browse",
                                     browseMetadataCallback,
                                     data,
                                     /* IN args */
                                     "ObjectID",
                                     G_TYPE_STRING,
                                     data->id,
                                     "BrowseFlag",
                                     G_TYPE_STRING,
                                     "BrowseMetadata",
                                     "Filter",
                                     G_TYPE_STRING,
                                     "*",
                                     "StartingIndex",
                                     G_TYPE_UINT,
                                     0,
                                     "RequestedCount",
                                     G_TYPE_UINT,
                                     0,
                                     "SortCriteria",
                                     G_TYPE_STRING,
                                     "",
                                     nullptr);
}

void UPnPInstance::appendDidlObject(GUPnPDIDLLiteObject *object, BrowseData *browseData, const char* udn)
{
    const char *id = nullptr;
    const char *parentId = nullptr;
    const char *title = nullptr;
    id = gupnp_didl_lite_object_get_id (object);
    title = gupnp_didl_lite_object_get_title (object);
    parentId = gupnp_didl_lite_object_get_parent_id (object);
    if (id == nullptr || title == nullptr || parentId == nullptr) {
        return;
    }
    browseMetadata((GUPnPServiceProxy*)g_object_ref(browseData->contentDir), id, udn, nullptr);
}

void UPnPInstance::onDidlObjectAvailable(GUPnPDIDLLiteParser *parser, GUPnPDIDLLiteObject *object, gpointer userdata)
{
    Q_UNUSED(parser);
    BrowseData *browseData;
    const char *udn;

    browseData = (BrowseData *)userdata;
    udn = gupnp_service_info_get_udn(GUPNP_SERVICE_INFO (browseData->contentDir));
    appendDidlObject(object, browseData, udn);
}

void UPnPInstance::browseDirectChildrenCallback(GUPnPServiceProxy *contentDir, GUPnPServiceProxyAction *action, gpointer userData)
{
    char *didlXml = nullptr;
    GError *error = nullptr;
    guint32 totalMatches;
    guint32 numberReturned;
    BrowseData *data = (BrowseData *)userData;

    gupnp_service_proxy_end_action(contentDir,
                                   action,
                                   &error,
                                   /* OUT args */
                                   "Result",
                                   G_TYPE_STRING,
                                   &didlXml,
                                   "NumberReturned",
                                   G_TYPE_UINT,
                                   &numberReturned,
                                   "TotalMatches",
                                   G_TYPE_UINT,
                                   &totalMatches,
                                   nullptr);
    if (error) {
        GUPnPServiceInfo *info;
        info = GUPNP_SERVICE_INFO(contentDir);
        g_warning("Failed to browse '%s': %s",
                  gupnp_service_info_get_location (info),
                  error->message);
        g_error_free(error);
    } else if (didlXml) {
        qDebug() << "DIDLXML:" << didlXml;

        GUPnPDIDLLiteParser *parser;
        parser = gupnp_didl_lite_parser_new();
        g_signal_connect(parser,
                         "object-available",
                         G_CALLBACK(UPnPInstance::onDidlObjectAvailable),
                         data);
        gupnp_didl_lite_parser_parse_didl(parser, didlXml, &error);
        gint32 remaining;
        gint32 batchSize;

        if (numberReturned > 0) {
            QDomElement docElem;
            QDomDocument xmlDoc;
            xmlDoc.setContent(QByteArray(didlXml));
            docElem=xmlDoc.documentElement();

            QDomNodeList containerList  = docElem.elementsByTagName("container");
            for (int i = 0; i < containerList.count(); i++) {
                QDomElement container = containerList.at(i).toElement();
                const char* id = g_strdup(container.attribute("id").toAscii().constData());
                browseDirectChildren(contentDir, id, 0, MAX_BROWSE);
            }
        }
        g_free(didlXml);
        data->startingIndex += numberReturned;
        remaining = totalMatches - data->startingIndex;
        if ((remaining > 0 || totalMatches == 0) && numberReturned != 0) {
            if (remaining > 0) {
                batchSize = MIN(remaining, MAX_BROWSE);
            } else {
                batchSize = MAX_BROWSE;
            }
            browseDirectChildren(data->contentDir,
                                 data->id,
                                 data->startingIndex,
                                 batchSize);
        }
    } 
}
