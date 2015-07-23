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

#include "upnpinstance.h"
#include "upnpmediasource.h"
#include "mediacenter/mediacenter.h"
#include "mediacenter/medialibrary.h"
#include "mediacenter/singletonfactory.h"

#include <QDebug>
#include <QTimer>
#include <QPair>

#define MEDIA_SERVER "urn:schemas-upnp-org:device:MediaServer:1"

GUPnPContextManager *UPnPMediaSource::contextManager;
QList< QPair< QString, UPnPInstance* > > UPnPMediaSource::mediaservers; 
int UPnPMediaSource::port = 0;

MEDIACENTER_EXPORT_MEDIASOURCE(UPnPMediaSource, "upnp.json")

UPnPMediaSource::UPnPMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{
}

void UPnPMediaSource::run()
{
    QTimer::singleShot(0, this, SLOT(setupContextManager()));
    exec();
}

void UPnPMediaSource::setupContextManager()
{
    contextManager = gupnp_context_manager_create(port);

    g_signal_connect(contextManager,
                     "context-available",
                     G_CALLBACK(UPnPMediaSource::onContextAvailable),
                     NULL);
}

void UPnPMediaSource::onContextAvailable(GUPnPContextManager *contextManager, GUPnPContext *context, gpointer userData)
{
    Q_UNUSED(userData);
    GUPnPControlPoint *mediaServerControlPoint;
    mediaServerControlPoint = gupnp_control_point_new(context, MEDIA_SERVER);

    g_signal_connect(mediaServerControlPoint,
                     "device-proxy-available",
                     G_CALLBACK(UPnPMediaSource::mediaServerProxyAvailable),
                     NULL);

    g_signal_connect(mediaServerControlPoint,
                     "device-proxy-unavailable",
                     G_CALLBACK(UPnPMediaSource::mediaServerProxyUnavailable),
                     NULL);

    gssdp_resource_browser_set_active(GSSDP_RESOURCE_BROWSER(mediaServerControlPoint), TRUE);
    gupnp_context_manager_manage_control_point(contextManager, mediaServerControlPoint);
}

void UPnPMediaSource::mediaServerProxyAvailable(GUPnPControlPoint *cp, GUPnPDeviceProxy  *proxy)
{
    Q_UNUSED(cp);

    QString udn = QString::fromLocal8Bit(gupnp_device_info_get_udn(GUPNP_DEVICE_INFO(proxy)));
    if(udn == "uuid:adsfa344-6b8b-4f60-87ca-228c46b5b0e8"){
        qDebug() << "Ignored PMC Media Server" << udn;
        return;
    }

    addMediaServer(proxy);
}

void UPnPMediaSource::mediaServerProxyUnavailable(GUPnPControlPoint *cp, GUPnPDeviceProxy  *proxy)
{
    Q_UNUSED(cp);
    removeMediaServer(proxy);
}

void UPnPMediaSource::addMediaServer(GUPnPDeviceProxy *proxy)
{
    QString udn;
    udn = gupnp_device_info_get_udn(GUPNP_DEVICE_INFO (proxy));
    Q_FOREACH(auto p, mediaservers) {
        if (p.first == udn) {
            qDebug() << "Media Server already added";
            return;
        }
    }
    qDebug() << "Adding media server proxy." << udn;

    UPnPInstance *n = new UPnPInstance(proxy);
    mediaservers.append(qMakePair(udn, n));
}

void UPnPMediaSource::removeMediaServer(GUPnPDeviceProxy *proxy)
{
    QString udn = QString::fromLocal8Bit(gupnp_device_info_get_udn(GUPNP_DEVICE_INFO (proxy)));
    qDebug() << "Removing media server proxy" << udn;
    MediaLibrary *mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();
    for(int i = 0; i < mediaList.size(); i++) {
        auto p = mediaList[i];
        if (p.first == udn) {
//             ToDo: Needs merging ashish/medialibrary
//             mediaLibrary->removeMedia(p.second);
            mediaList.removeAt(i);
            i--;
        }
    }
    for(int i = 0; i < mediaservers.size(); i++) {
        auto p = mediaservers[i];
        if (p.first == udn) {
            delete p.second;
            mediaservers.removeAt(i);
            break;
        }
    }
}

void UPnPMediaSource::rescanMediaServer(GUPnPDeviceProxy *proxy)
{
    removeMediaServer(proxy);
    addMediaServer(proxy);
}

void UPnPMediaSource::addMedia(QHash<int, QString> properties)
{
    MediaLibrary *mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();
    MediaHandler *mediaHandler = new MediaHandler(mediaLibrary, 0);
    mediaHandler->handleResult(properties);
}

#include "upnpmediasource.moc"
