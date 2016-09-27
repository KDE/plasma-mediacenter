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

#include "kmediacollectionplugin.h"

#include "subtitleprovider.h"
#include "filterplaylistmodel.h"
#include "multipleplaylistmodel.h"
#include "objectpair.h"
#include "settings.h"
#include "datasourcesloader.h"
#include "abstractbrowsingbackend.h"
#include "medialibrary.h"
#include "runtimedata.h"
#include "pmccoverartprovider.h"
#include "playlistmodel.h"
#include "mpris2/mediaplayer2player.h"
#include "mediabrowserloader.h"

#include "qmlaccess.h"
#include <iconimageprovider.h>

void KMediaCollectionPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.mediacollection"));

    qmlRegisterType<BackendsModel>(uri, 2, 0, "BackendsModel");
    qmlRegisterType<SubtitleProvider>(uri, 2, 0, "SubtitleProvider");
    qmlRegisterType<FilterPlaylistModel>(uri, 2, 0, "FilterPlaylistModel");
    qmlRegisterType<MultiplePlaylistModel>(uri, 2, 0, "MultiplePlaylistModel");
    qmlRegisterType<ObjectPair>(uri, 2, 0, "ObjectPair");
    qmlRegisterType<QMLAccess>(uri, 2, 0, "QMLAccess");
    qmlRegisterType<Settings>(uri, 2, 0, "Settings");
    qmlRegisterType<RuntimeData>(uri, 2, 0, "RuntimeData");
    qmlRegisterType<MediaBrowserLoader>(uri, 2, 0, "MediaBrowserLoader");

    qRegisterMetaType<PlaylistModel*>("PlaylistModel*");
    qRegisterMetaType<KMediaCollection::AbstractBrowsingBackend*>("KMediaCollection::AbstractBrowsingBackend*");
    qRegisterMetaType<MediaPlayer2Player*>("MediaPlayer2Player*");

}

void KMediaCollectionPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_ASSERT(uri == QLatin1String("org.kde.plasma.mediacollection"));
    engine->addImageProvider(PmcCoverArtProvider::identificationString, new PmcCoverArtProvider());
    engine->addImageProvider(IconImageProvider::identificationString, new IconImageProvider());
}

#include "moc_mediacollectionplugin.cpp"
