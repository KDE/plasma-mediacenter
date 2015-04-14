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

extern "C" {
    #include <libgupnp-av/gupnp-av.h>
    #include <libgssdp/gssdp.h>
    #include <gtk/gtk.h>
    GUPnPContext *context;
    GUPnPRootDevice *device;
    int upnpServerPort = 34552;
    int id = 1;
}


#include "mediaserver.h"
#include "singletonfactory.h"
#include "medialibrary.h"
#include "pmcmedia.h"

#include <QStandardPaths>
#include <QDebug>

static char*
addItem (const char *mime, const char *title, const char *path)
{
    GUPnPContext *context;
    const char *didl;
    char *result;
    GUPnPDIDLLiteWriter *didlw = gupnp_didl_lite_writer_new (NULL);
    GUPnPDIDLLiteObject *object = GUPNP_DIDL_LITE_OBJECT
            (gupnp_didl_lite_writer_add_item (didlw));
    context = gupnp_device_info_get_context (GUPNP_DEVICE_INFO (device));

    GUPnPDIDLLiteResource *res;
    res = gupnp_didl_lite_object_add_resource(object);
    gupnp_didl_lite_resource_set_uri(res, g_strdup_printf ("http://%s:%d%s",
                                                           gupnp_context_get_host_ip (context),
                                                           gupnp_context_get_port (context),
                                                           path));
    GUPnPProtocolInfo *protocolInfo =
            gupnp_protocol_info_new_from_string(g_strdup_printf("http-get:*:%s:*", mime), NULL);
    gupnp_didl_lite_resource_set_protocol_info(res, protocolInfo);
    gupnp_didl_lite_object_set_id(object, QString::number(id++).toStdString().c_str());
    gupnp_didl_lite_object_set_parent_id(object, "0");
    gupnp_didl_lite_object_set_title(object, title);
    gupnp_didl_lite_object_set_restricted(object, true);

    didl = gupnp_didl_lite_writer_get_string (didlw);
    result = g_strdup(didl);
    return result;
}

static void browse_cb(GUPnPService *service,
                      GUPnPServiceAction *action,
                      gpointer user_data)
{
    char *object_id, *browse_flag;
    const char* finalResult;
    guint num_returned = 0;

    /* Handle incoming arguments */
    gupnp_service_action_get (action,
                              "ObjectID",
                              G_TYPE_STRING,
                              &object_id,
                              "BrowseFlag",
                              G_TYPE_STRING,
                              &browse_flag,
                              NULL);
    QString result;
    MediaLibrary *library = SingletonFactory::instanceFor<MediaLibrary>();
    QStringList mediaType = {"audio", "video", "image"};
    foreach (QString type, mediaType) {
        qDebug() << "Media Type" << type;
        QList < QSharedPointer <PmcMedia> > mediaList = library->getMedia(type);
        foreach(QSharedPointer <PmcMedia> media, mediaList){
            qDebug() << "URL" << media->url();
            const char *mimeType;
            if (type == "audio") {
                mimeType = "audio/mp3";
            } else if(type == "video") {
                mimeType = "video/mp4";
            } else {
                mimeType = "image/png";
            }
            result += QString::fromUtf8(addItem(mimeType, media->title().toStdString().c_str(), media->url().toStdString().c_str()));
            num_returned++;
        }
    }
    qDebug() << "Result" << result;

    finalResult = result.toStdString().c_str();

    /* Set action return arguments */
    gupnp_service_action_set (action,
                              "Result",
                              G_TYPE_STRING,
                              finalResult,
                              "NumberReturned",
                              G_TYPE_UINT,
                              num_returned,
                              "TotalMatches",
                              G_TYPE_UINT,
                              num_returned,
                              "UpdateID",
                              G_TYPE_UINT,
                              32,
                              NULL);

    gupnp_service_action_return (action);
}

static void
server_callback (SoupServer        *server,
                 SoupMessage       *msg,
                 const char        *path,
                 GHashTable        *query,
                 SoupClientContext *client,
                 gpointer           user_data)
{
    GMappedFile *mapping;
    SoupBuffer *buffer;

    mapping = g_mapped_file_new (path, FALSE, NULL);
    if (!mapping) {
        soup_message_set_status (msg, SOUP_STATUS_INTERNAL_SERVER_ERROR);
        return;
    }
    buffer = soup_buffer_new_with_owner (g_mapped_file_get_contents (mapping),
                                         g_mapped_file_get_length (mapping),
                                         mapping,
                                         (GDestroyNotify)g_mapped_file_unref);

    soup_message_body_append_buffer (msg->response_body, buffer);
    soup_buffer_free (buffer);
    soup_message_set_status (msg, SOUP_STATUS_OK);
}

MediaServer::MediaServer()
{
    context = gupnp_context_new(NULL,
                                NULL,
                                upnpServerPort,
                                NULL);

    gpointer user_data;
    gupnp_context_add_server_handler(context,
                                     FALSE,
                                     "/",
                                     server_callback,
                                     user_data,
                                     NULL);
    QString mediaServerSpecsPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "plasma/mediacenter/MediaServer1.xml");
    if(mediaServerSpecsPath.isEmpty()){
        qDebug() << "MediaServer1.xml not found, starting media server failed!";
        return;
    }
    device = gupnp_root_device_new(context,
                                   "MediaServer1.xml",
                                   mediaServerSpecsPath.split("MediaServer1.xml")[0].toLocal8Bit().constData());

    gupnp_root_device_set_available(device, TRUE);

    GUPnPServiceInfo *service;
    service = gupnp_device_info_get_service(GUPNP_DEVICE_INFO (device),
                                            "urn:schemas-upnp-org:service:ContentDirectory");

    g_signal_connect(service,
                     "action-invoked::Browse",
                     G_CALLBACK (browse_cb),
                     NULL);
}
