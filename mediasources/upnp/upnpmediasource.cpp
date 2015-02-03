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

extern "C"{
    #include <libgupnp/gupnp-control-point.h>
    #include <libgupnp-av/gupnp-av.h>
    #include <libgssdp/gssdp.h>
    #define CONTENT_DIR "urn:schemas-upnp-org:service:ContentDirectory"
    #define MAX_BROWSE 64
    #define MEDIA_SERVER "urn:schemas-upnp-org:device:MediaServer:1"
    typedef struct{
        GUPnPServiceProxy *contentDir;
        gchar *id;
        guint32 startingIndex;
    } BrowseData;
    int upnpClientPort = 0;
    GUPnPContextManager *contextManager;
}

#include "upnpmediasource.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/mediacenter.h>

#include <QDebug>
#include <QDomElement>
#include <QDomDocument>
#include <QTimer>

void addMedia(QHash<int, QString> properties);

static void browse(GUPnPServiceProxy *contentDir,
                   const char        *containerId,
                   guint32            startingIndex,
                   guint32            requestedCount);

static GUPnPServiceProxy* getContentDir(GUPnPDeviceProxy *proxy)
{
    GUPnPDeviceInfo *info;
    GUPnPServiceInfo *contentDir;
    info = GUPNP_DEVICE_INFO(proxy);
    contentDir = gupnp_device_info_get_service(info, CONTENT_DIR);
    return GUPNP_SERVICE_PROXY(contentDir);
}

static void appendDidlObject(GUPnPDIDLLiteObject *object,
                             BrowseData          *browseData,
                             const char*         udn)
{
    Q_UNUSED(udn);
    QString id, title, parentId;
    bool isContainer;
    id = gupnp_didl_lite_object_get_id (object);
    title = gupnp_didl_lite_object_get_title (object);
    parentId = gupnp_didl_lite_object_get_parent_id (object);

    if (id.isEmpty() || title.isEmpty() || parentId.isEmpty()) {
        return;
    }

    isContainer = GUPNP_IS_DIDL_LITE_CONTAINER (object);

    if (isContainer) {
        if (strcmp(browseData->id, "0") == 0) {
            #ifdef UPNP_DEBUG
            qDebug() << "Root Container!";
            #endif
        }
        browse(browseData->contentDir, id.toLocal8Bit(), 0, MAX_BROWSE);
    }
    else {
        QString artist, album, albumArtist;
        artist = gupnp_didl_lite_object_get_artist (object);
        album = gupnp_didl_lite_object_get_album (object);
        albumArtist = gupnp_didl_lite_object_get_author(object);
//         ToDo : Get duration property of media
//         xmlNodeGetContent(gupnp_didl_lite_object_get_xml_node(object));
        QHash<int, QString> properties;
//         properties.insert(0, url);
        properties.insert(1, title);
//         properties.insert(2, duration);
        properties.insert(3, artist);
        properties.insert(4, album);
        properties.insert(5, albumArtist);
        addMedia(properties);
    }
}

static void onDidlObjectAvailable(GUPnPDIDLLiteParser *parser,
                                  GUPnPDIDLLiteObject *object,
                                  gpointer             userData)
{
    Q_UNUSED(parser);
    BrowseData *browseData;
    const char *udn;

    browseData = (BrowseData *)userData;
    udn = gupnp_service_info_get_udn(GUPNP_SERVICE_INFO(browseData->contentDir));
    appendDidlObject(object,
                     browseData,
                     udn);
}

static void browseCallBack(GUPnPServiceProxy       *contentDir,
                           GUPnPServiceProxyAction *action,
                           gpointer                 userData)
{
    BrowseData *data;
    char       *didlXml;
    guint32     numberReturned;
    guint32     totalMatches;
    GError     *error;
    data = (BrowseData *)userData;
    didlXml = NULL;
    error = NULL;

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
                                   NULL);

    if (didlXml) {
        GUPnPDIDLLiteParser *parser;
        gint32 remaining;
        gint32 batchSize;
        GError *error;

        error = NULL;
        parser = gupnp_didl_lite_parser_new();

        g_signal_connect(parser,
                         "object-available",
                         G_CALLBACK (onDidlObjectAvailable),
                         data);

        /* Only try to parse DIDL if server claims that there is result */
        if (numberReturned > 0) {
            if (!gupnp_didl_lite_parser_parse_didl(parser,
                                                   didlXml,
                                                   &error)) {
                g_warning ("Error while browsing %s: %s",
                           data->id,
                           error->message);
                g_error_free (error);
            }
        }
        g_object_unref(parser);
        g_free(didlXml);

        data->startingIndex += numberReturned;

        /* See if we have more objects to get */
        remaining = totalMatches - data->startingIndex;

        /* Keep browsing till we get each and every object */
        if ((remaining > 0 || totalMatches == 0) && numberReturned != 0) {
            if (remaining > 0) {
                batchSize = MIN(remaining, MAX_BROWSE);
            }
            else{
                batchSize = MAX_BROWSE;
            }
            browse(contentDir,
                   data->id,
                   data->startingIndex,
                   batchSize);
        }
    }
    else if (error) {
        GUPnPServiceInfo *info;
        info = GUPNP_SERVICE_INFO(contentDir);
        g_warning("Failed to browse '%s': %s",
                  gupnp_service_info_get_location (info),
                  error->message);
        g_error_free(error);
    }
}

static void browse(GUPnPServiceProxy *contentDir,
                   const char        *containerId,
                   guint32            startingIndex,
                   guint32            requestedCount)
{
    BrowseData *data;
    data = g_slice_new(BrowseData);
    data->contentDir = (GUPnPServiceProxy*) g_object_ref(contentDir);
    data->id = g_strdup(containerId);
    data->startingIndex = startingIndex;

    #ifdef UPNP_DEBUG
    qDebug() << "Container ID and Starting Index if item is container:" << containerId << startingIndex << endl;
    #endif

    gupnp_service_proxy_begin_action(contentDir,
                                     "Browse",
                                     browseCallBack,
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
                                     NULL);
}

static void appendMediaServer(GUPnPDeviceProxy *proxy)
{

    GUPnPDeviceInfo *info;
    GUPnPServiceProxy *contentDir;
    QString friendlyName;

    info = GUPNP_DEVICE_INFO(proxy);
    contentDir = getContentDir(proxy);
    friendlyName = gupnp_device_info_get_friendly_name(info);

    #ifdef UPNP_DEBUG
    qDebug() << "Friendly Name" << friendlyName;
    #endif

    browse(contentDir, "0", 0, MAX_BROWSE);
    gupnp_service_proxy_set_subscribed(contentDir, TRUE);
    g_object_unref(contentDir);
}

void addMediaServer(GUPnPDeviceProxy *proxy)
{
    QString udn;
    udn = gupnp_device_info_get_udn(GUPNP_DEVICE_INFO (proxy));

    #ifdef UPNP_DEBUG
    qDebug() << "UDN :" << udn;
    #endif

    //ToDo: Check if media server is already added
    appendMediaServer(proxy);
}

void removeMediaServer(GUPnPDeviceProxy *proxy)
{
    QString udn;
    udn = gupnp_device_info_get_udn(GUPNP_DEVICE_INFO (proxy));
    //ToDo: Remove Media Server
}

static void mediaServerProxyAvailable(GUPnPControlPoint *cp,
                                      GUPnPDeviceProxy  *proxy)
{
    Q_UNUSED(cp);
    #ifdef UPNP_DEBUG
    qDebug() << "Adding media server proxy.";
    #endif

    addMediaServer (proxy);
}

static void mediaServerProxyUnavailable(GUPnPControlPoint *cp,
                                        GUPnPDeviceProxy  *proxy)
{
    Q_UNUSED(cp);
    #ifdef UPNP_DEBUG
    qDebug() << "Removing media server proxy.";
    #endif

    removeMediaServer(proxy);
}

static void onContextAvailable(GUPnPContextManager *contextManager,
                               GUPnPContext        *context,
                               gpointer             userData)
{
    Q_UNUSED(userData);
    GUPnPControlPoint *mediaServerControlPoint;
    mediaServerControlPoint = gupnp_control_point_new(context, MEDIA_SERVER);

    g_signal_connect(mediaServerControlPoint,
                     "device-proxy-available",
                     G_CALLBACK (mediaServerProxyAvailable),
                     NULL);

    g_signal_connect(mediaServerControlPoint,
                     "device-proxy-unavailable",
                     G_CALLBACK (mediaServerProxyUnavailable),
                     NULL);

    gssdp_resource_browser_set_active(GSSDP_RESOURCE_BROWSER (mediaServerControlPoint),
                                       TRUE);
    gupnp_context_manager_manage_control_point(contextManager, mediaServerControlPoint);
    g_object_unref(mediaServerControlPoint);
}

void UPnPMediaSource::setupContextManager()
{
    contextManager = gupnp_context_manager_create(upnpClientPort);

    g_signal_connect(contextManager,
                     "context-available",
                     G_CALLBACK (onContextAvailable),
                     NULL);
}

UPnPMediaSource::UPnPMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{
}

void UPnPMediaSource::run()
{
    QTimer::singleShot(0, this, SLOT(setupContextManager()));
    exec();
}

void UPnPMediaSource::addMedia(QHash<int, QString> properties)
{
    MediaLibrary *mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();
    MediaHandler *mediaHandler = new MediaHandler(mediaLibrary, 0);
    mediaHandler->handleResult(properties);
}
