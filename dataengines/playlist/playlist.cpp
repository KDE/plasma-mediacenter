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

#include "playlistservice.h"

#include <KDebug>
#include <KGlobal>
#include <KConfig>
#include <KConfigGroup>

#include <QFileInfo>
#include <QHash>

PlaylistEngine::PlaylistEngine(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args)
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
}

void PlaylistEngine::init()
{
    // here we load the playlists from the config if previously stored
    KConfig c("playlistenginerc");
    KConfigGroup g(&c, "Playlists");
    foreach (const QString &key, g.keyList()) {
        addToPlaylist(key, g.readEntry(key, QStringList()));
    }
}

Plasma::Service* PlaylistEngine::serviceForSource(const QString &source)
{
    kDebug() << "creating service";
    if (!sources().contains(source)) {
        return 0;
    }

    PlaylistService *service = new PlaylistService(this, this);
    service->setDestination(source);

    return service;
}

bool PlaylistEngine::sourceRequestEvent(const QString &name)
{
    if (sources().contains(name)) {
        return true;
    }

    setData(name, Plasma::DataEngine::Data());
    return true;
}

bool PlaylistEngine::updateSourceEvent(const QString &name)
{
    int edit = 0;

    kDebug() << "verifying consitence";
    // if a file in the playlist does not exist anymore
    // we remove it from the exposed data.
    QHash<QString, QStringList>::Iterator it;
    for (it = m_playlists.begin(); it != m_playlists.end(); ++it) {
        int singleChanged = 0;
        foreach (const QString &file, it.value()) { // scanning through each file of each playlist
            if (!QFileInfo(file).exists()) {
                QStringList values = it.value();
                QString key = it.key();
                it = m_playlists.erase(it);
                values.removeAll(file);
                it = m_playlists.insert(key, values);
                ++edit;
                ++singleChanged;
            }
        }
        saveToConfig(it.key(), it.value());
        setData(it.key(), it.value());
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
    m_playlists[playlistName] << files;

    // setting data to the engine
    setData(playlistName, m_playlists.value(playlistName));

    // storing files in the config
    saveToConfig(playlistName, m_playlists[playlistName]);

    // update the current playlist
    if (m_current == playlistName) {
        setCurrentPlaylist(playlistName);
    }
}

void PlaylistEngine::addToPlaylist(const QString &playlistName, const QString &file)
{
    addToPlaylist(playlistName, QStringList() << file);
}

QStringList PlaylistEngine::availablePlaylists()
{
    return m_playlists.keys();
}

QStringList PlaylistEngine::filesInPlaylist(const QString &playlistName)
{
    return m_playlists[playlistName];
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

    if (!m_playlists.keys().contains(playlistName)) {
        return;
    }

    removeAllData(playlistName);

    QStringList list = m_playlists[playlistName];
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

    m_playlists.remove(playlistName);
    if (list.isEmpty()) {
        removeFromConfig(playlistName);
        return;
    }

    m_playlists[playlistName] = list;

    setData(playlistName, m_playlists.value(playlistName));

    saveToConfig(playlistName, m_playlists.value(playlistName));
}

void PlaylistEngine::removeFromPlaylist(const QString &playlistName, const QString &file)
{
    removeFromPlaylist(playlistName, QStringList() << file);
}

void PlaylistEngine::removePlaylist(const QString &playlistName)
{
    if (!m_playlists.keys().contains(playlistName)) {
        return;
    }

    removeAllData(playlistName);
    m_playlists.remove(playlistName);
    removeFromConfig(playlistName);
}

void PlaylistEngine::setCurrentPlaylist(const QString &playlistName)
{
    if (!sources().contains(playlistName)) {
        kDebug() << "no such" << playlistName << "found!";
        return;
    }

    m_current = playlistName;
    setData("currentPlaylist", m_playlists[playlistName]);
    emit currentPlaylistChanged();
}

void PlaylistEngine::saveToConfig(const QString &playlist, const QStringList &files)
{
    KConfig c("playlistenginerc");
    KConfigGroup g(&c, "Playlists");
    g.writeEntry(playlist, files);
}

void PlaylistEngine::removeFromConfig(const QString &playlist)
{
    KConfig c("playlistenginerc");
    KConfigGroup g(&c, "Playlists");
    g.deleteEntry(playlist);
}

K_EXPORT_PLASMA_DATAENGINE(playlist, PlaylistEngine)
