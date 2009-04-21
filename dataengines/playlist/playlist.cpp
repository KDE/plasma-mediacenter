/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "playlist.h"
#include "playlistengineadaptor.h"

#include <KDebug>

#include <QFileInfo>
#include <QHash>

static const char* SOURCE_NAME = "playlist";

class PlaylistEngine::Private
{
public:
Private(PlaylistEngine *q) : q(q)
{}
~Private()
{ playlists.clear(); }

PlaylistEngine *q;
// we allow many playlists in the dataengine.
// Each playlist is named with a QString.
QHash<QString, QStringList> playlists;
};

PlaylistEngine::PlaylistEngine(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args), d(new Private(this))
{
    new PlaylistEngineAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerService("org.kde.PlaylistEngine");
    dbus.registerObject("/PlaylistEngine", this);

    // We just use this interval to check wether the
    // specified urls for the playlist have not been
    // deleted in the last 5 secs.
    setMinimumPollingInterval(5000);
}

PlaylistEngine::~PlaylistEngine()
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.unregisterService("org.kde.PlaylistEngine");

    delete d;
}

void PlaylistEngine::init()
{
    kDebug() << "engine correctly initializated";
}

bool PlaylistEngine::sourceRequestEvent(const QString &name)
{
    // We only offer a source
    if (name != SOURCE_NAME) {
        return false;
    }

    return updateSourceEvent(name);
}

bool PlaylistEngine::updateSourceEvent(const QString &name)
{
    if (name != SOURCE_NAME) {
        return false;
    }

    int edit = 0;

    // if a file in the playlist does not exist anymore
    // we remove it from the exposed data.
    QHash<QString, QStringList>::Iterator it;
    for (it = d->playlists.begin(); it != d->playlists.end(); ++it) {
        foreach (const QString &file, it.value()) { // scanning through each file of each playlist
            if (!QFileInfo(file).exists()) {
                removeData(SOURCE_NAME, it.key());
                ++edit;
            }
        }
    }

    return (edit > 0);
}

void PlaylistEngine::addToPlaylist(const QString &playlistName, const QString &file)
{
    if (!QFileInfo(file).exists()) {
        return;
    }

    d->playlists[playlistName] << file;
    setData(SOURCE_NAME, playlistName, d->playlists.value(playlistName));
}

QStringList PlaylistEngine::availablePlaylists()
{
    return d->playlists.keys();
}

K_EXPORT_PLASMA_DATAENGINE(playlist, PlaylistEngine)
