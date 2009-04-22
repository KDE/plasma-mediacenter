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
#include <KGlobal>
#include <KConfig>
#include <KConfigGroup>

#include <QFileInfo>
#include <QHash>

static const char* KEY = "media file";

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

public:
void saveToConfig(const QString &playlist, const QStringList &files);
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

    // here we load the playlists from the config if previously stored
    KConfig c("playlistenginerc");
    KConfigGroup g(&c, "Playlists");
    foreach (const QString &key, g.keyList()) {
        addToPlaylist(key, g.readEntry(key, QStringList()));
    }
}

bool PlaylistEngine::sourceRequestEvent(const QString &name)
{
    return updateSourceEvent(name);
}

bool PlaylistEngine::updateSourceEvent(const QString &name)
{
    int edit = 0;

    kDebug() << "verifying consitence";
    // if a file in the playlist does not exist anymore
    // we remove it from the exposed data.
    QHash<QString, QStringList>::Iterator it;
    for (it = d->playlists.begin(); it != d->playlists.end(); ++it) {
        int singleChanged = 0;
        foreach (const QString &file, it.value()) { // scanning through each file of each playlist
            if (!QFileInfo(file).exists()) {
                QStringList values = it.value();
                QString key = it.key();
                it = d->playlists.erase(it);
                values.removeAll(file);
                it = d->playlists.insert(key, values);
                ++edit;
                ++singleChanged;
            }
        }
        d->saveToConfig(it.key(), it.value());
        setData(it.key(), KEY, it.value());
        singleChanged = 0;
    }

    

    return (edit > 0);
}

void PlaylistEngine::addToPlaylist(const QString &playlistName, QStringList files)
{
    kDebug() << "adding to playlist";
    // removing invalid entries
    foreach (const QString &file, files) {
        if (!QFileInfo(file).exists()) {
            files.removeAll(file);
        }
    }

    if (files.isEmpty()) {
        kDebug() << "empty list";
        return;
    }

    // adding files to memory
    d->playlists[playlistName] << files;

    // storing files in the config
    d->saveToConfig(playlistName, d->playlists[playlistName]);

    // setting data to the engine
    setData(playlistName, KEY, d->playlists.value(playlistName));
}

void PlaylistEngine::addToPlaylist(const QString &playlistName, const QString &file)
{
    addToPlaylist(playlistName, QStringList() << file);
}

QStringList PlaylistEngine::availablePlaylists()
{
    return d->playlists.keys();
}

QStringList PlaylistEngine::filesInPlaylist(const QString &playlistName)
{
    return d->playlists[playlistName];
}

void PlaylistEngine::removeFromPlaylist(const QString &playlistName, QStringList files)
{
    // removing invalid entries
    foreach (const QString &file, files) {
        if (!QFileInfo(file).exists()) {
            files.removeAll(file);
        }
    }

    if (files.isEmpty()) {
        kDebug() << "empty list";
        return;
    }

    if (!d->playlists.keys().contains(playlistName)) {
        return;
    }

    removeAllData(playlistName);

    QStringList list = d->playlists[playlistName];
    int changed = 0;
    foreach (const QString &file, files) {
        if (!list.removeOne(file)) {
            continue;
        }
        ++changed;
    }

    if (changed == 0) {
        return;
    }

    d->playlists.remove(playlistName);
    if (list.isEmpty()) {
        KConfig c("playlistenginerc");
        KConfigGroup g(&c, "Playlists");
        g.deleteEntry(playlistName);
        return;
    }

    d->playlists[playlistName] = list;

    setData(playlistName, KEY, d->playlists.value(playlistName));

    d->saveToConfig(playlistName, d->playlists.value(playlistName));
}

void PlaylistEngine::removeFromPlaylist(const QString &playlistName, const QString &file)
{
    removeFromPlaylist(playlistName, QStringList() << file);
}

void PlaylistEngine::Private::saveToConfig(const QString &playlist, const QStringList &files)
{
    KConfig c("playlistenginerc");
    KConfigGroup g(&c, "Playlists");
    g.writeEntry(playlist, files);
}

K_EXPORT_PLASMA_DATAENGINE(playlist, PlaylistEngine)
