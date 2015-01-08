/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "mediacenterplugin.h"

#include "mediacenter/subtitleprovider.h"
#include "mediacenter/filterplaylistmodel.h"
#include "mediacenter/multipleplaylistmodel.h"
#include "mediacenter/objectpair.h"
#include "mediacenter/settings.h"
#include "mediacenter/mediasourcesloader.h"
#include "mediacenter/abstractbrowsingbackend.h"
#include "mediacenter/medialibrary.h"
#include "mediacenter/runtimedata.h"
#include "mediacenter/pmccoverartprovider.h"
#include "mediacenter/playlistmodel.h"
#include "mediacenter/backendsloader.h"
#include "mediacenter/mpris2/mediaplayer2player.h"

#include "qmlaccess.h"
#include <iconimageprovider.h>

void MediaCenterPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.mediacenter"));

    qmlRegisterType<BackendsModel>(uri, 2, 0, "BackendsModel");
    qmlRegisterType<SubtitleProvider>(uri, 2, 0, "SubtitleProvider");
    qmlRegisterType<FilterPlaylistModel>(uri, 2, 0, "FilterPlaylistModel");
    qmlRegisterType<MultiplePlaylistModel>(uri, 2, 0, "MultiplePlaylistModel");
    qmlRegisterType<ObjectPair>(uri, 2, 0, "ObjectPair");
    qmlRegisterType<QMLAccess>(uri, 2, 0, "QMLAccess");
    qmlRegisterType<Settings>(uri, 2, 0, "Settings");
    qmlRegisterType<RuntimeData>(uri, 2, 0, "RuntimeData");
    qmlRegisterType<BackendsLoader>(uri, 2, 0, "BackendsLoader");

    qRegisterMetaType<PlaylistModel*>("PlaylistModel*");
    qRegisterMetaType<MediaCenter::AbstractBrowsingBackend*>("MediaCenter::AbstractBrowsingBackend*");
    qRegisterMetaType<MediaPlayer2Player*>("MediaPlayer2Player*");

}

void MediaCenterPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.mediacenter"));
    engine->addImageProvider(PmcCoverArtProvider::identificationString, new PmcCoverArtProvider());
    engine->addImageProvider(IconImageProvider::identificationString, new IconImageProvider());
}

#include "moc_mediacenterplugin.cpp"
