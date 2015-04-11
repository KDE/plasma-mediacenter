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

#define UPNP_DEBUG 1

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
        gint32 remaining;
        gint32 batchSize;

        if (numberReturned > 0) {
            QDomElement docElem;
            QDomDocument xmlDoc;
            xmlDoc.setContent(QByteArray(didlXml));
            docElem=xmlDoc.documentElement();
            QDomNodeList itemList = docElem.elementsByTagName("item");
            QString artist, album, albumArtist, title, mediaType, duration, url, protocolInfo, mimeType;

            for (int i = 0; i < itemList.count(); i++) {
                QDomElement item = itemList.at(i).toElement();
                title = item.elementsByTagName("dc:title").at(0).toElement().text();
                artist = item.elementsByTagName("upnp:artist").at(0).toElement().text();
                album = item.elementsByTagName("upnp:album").at(0).toElement().text();
                protocolInfo= item.elementsByTagName("res").at(0).toElement().attribute("protocolInfo");
                duration = item.elementsByTagName("res").at(0).toElement().attribute("duration");
                url = item.elementsByTagName("res").at(0).toElement().text();
                if (!protocolInfo.isEmpty()) {
                    mimeType = protocolInfo.split(":").at(2);
                }
                qDebug() << "Media item" << url;

                QHash<int, QString> properties;
                properties.insert(MediaCenter::MediaUrlRole, url);
                properties.insert(Qt::DisplayRole, title);
                properties.insert(MediaCenter::DurationRole, duration);
                properties.insert(MediaCenter::ArtistRole, artist);
                properties.insert(MediaCenter::AlbumRole, album);
                properties.insert(MediaCenter::AlbumArtistRole, albumArtist);
                properties.insert(MediaCenter::MimeTypeRole, mimeType);
                UPnPMediaSource::addMedia(properties);
            }
            QDomNodeList containerList  = docElem.elementsByTagName("container");
            for (int i = 0; i < containerList.count(); i++) {
                QDomElement container = containerList.at(i).toElement();
                const char* id = g_strdup(container.attribute("id").toAscii().constData());
                browse(contentDir, id, 0, MAX_BROWSE);
            }
        }
        g_free(didlXml);
        data->startingIndex += numberReturned;
        remaining = totalMatches - data->startingIndex;
        if ((remaining > 0 || totalMatches == 0) && numberReturned != 0) {
            if (remaining > 0) {
                batchSize = MIN(remaining, MAX_BROWSE);
            }
            else {
                batchSize = MAX_BROWSE;
            }
            browse(data->contentDir,
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

    QString udn = gupnp_device_info_get_udn(GUPNP_DEVICE_INFO (proxy));
    if(udn == "uuid:adsfa344-6b8b-4f60-87ca-228c46b5b0e8"){
        qDebug() << "ignored pmc media server";
        return;
    }

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
}

void UPnPMediaSource::setupContextManager()
{
    contextManager = gupnp_context_manager_create(upnpClientPort);

    g_signal_connect(contextManager,
                     "context-available",
                     G_CALLBACK (onContextAvailable),
                     NULL);
}

MEDIACENTER_EXPORT_MEDIASOURCE(UPnPMediaSource, "upnp.json")

UPnPMediaSource::UPnPMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{
    qDebug() << "UPnPMediaSource Constructor called!";
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

#include "upnpmediasource.moc"