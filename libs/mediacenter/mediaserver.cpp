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
    int upnpServerPort = 0;
}


#include "mediaserver.h"

static void
add_item (GUPnPContext        *context,
          GUPnPDIDLLiteWriter *didl_writer,
          GUPnPDIDLLiteObject *object,
          const char          *id,
          const char          *parent_id,
          const char          *mime,
          const char          *title,
          const char          *path)
{
    GUPnPDIDLLiteResource *res;
    res = gupnp_didl_lite_object_add_resource(object);
    gupnp_didl_lite_resource_set_uri(res, g_strdup_printf ("http://%s:%d%s",
                                                           gupnp_context_get_host_ip (context),
                                                           gupnp_context_get_port (context),
                                                           path));
    GUPnPProtocolInfo *protocolInfo =
            gupnp_protocol_info_new_from_string(g_strdup_printf("http-get:*:%s:*", mime), NULL);
    gupnp_didl_lite_resource_set_protocol_info(res, protocolInfo);
    gupnp_didl_lite_object_set_id(object, id);
    gupnp_didl_lite_object_set_parent_id(object, parent_id);
    gupnp_didl_lite_object_set_title(object, title);
    gupnp_didl_lite_object_set_restricted(object, true);
}

static char* browse_direct_children()
{
    GUPnPContext *context;
    const char *didl;
    char *result;
    GUPnPDIDLLiteWriter *didlw = gupnp_didl_lite_writer_new (NULL);
    GUPnPDIDLLiteObject *object = GUPNP_DIDL_LITE_OBJECT
            (gupnp_didl_lite_writer_add_item (didlw));
    context = gupnp_device_info_get_context (GUPNP_DEVICE_INFO (device));
    add_item (context,
              didlw,
              object,
              "4000",
              "0",
              "audio/mp3",
              "Song1",
              "/home/phantom/Music/2.mp3");
    didl = gupnp_didl_lite_writer_get_string (didlw);
    result = g_strdup(didl);
    return result;
}

static void browse_cb(GUPnPService *service,
                      GUPnPServiceAction *action,
                      gpointer user_data)
{
//     qDebug() << "Browse";
    char *object_id, *browse_flag;
    char *result;
    guint num_returned = 1;

    /* Handle incoming arguments */
    gupnp_service_action_get (action,
                              "ObjectID",
                              G_TYPE_STRING,
                              &object_id,
                              "BrowseFlag",
                              G_TYPE_STRING,
                              &browse_flag,
                              NULL);

    result = browse_direct_children();

    /* Set action return arguments */
    gupnp_service_action_set (action,
                              "Result",
                              G_TYPE_STRING,
                              result,
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
//     qDebug() << "File Requested" << (char*) user_data;

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

void MediaServer::updateLibrary()
{

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

    device = gupnp_root_device_new(context,
                                   "MediaServer1.xml",
                                   ".");

    gupnp_root_device_set_available(device, TRUE);

    GUPnPServiceInfo *service;
    service = gupnp_device_info_get_service(GUPNP_DEVICE_INFO (device),
                                            "urn:schemas-upnp-org:service:ContentDirectory");

    g_signal_connect(service,
                     "action-invoked::Browse",
                     G_CALLBACK (browse_cb),
                     NULL);
}
