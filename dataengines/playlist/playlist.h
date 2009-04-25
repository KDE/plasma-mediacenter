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
#ifndef PLAYLISTENGINE_H
#define PLAYLISTENGINE_H

#include <Plasma/DataEngine>

/**
 * @class PortageEngine
 * @brief An engine that manages your playlists.
 *
 * The engine exports each playlist as source of the Plasma::DataEngine.
 * @author Alessandro Diaferia
 */
class PlaylistEngine : public Plasma::DataEngine
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.PlaylistEngine")
public:
    PlaylistEngine(QObject *parent, const QVariantList &args);
    ~PlaylistEngine();

    void init();

public slots:
    void addToPlaylist(const QString &playlistName, QStringList files);
    void addToPlaylist(const QString &playlistName, const QString &file);
    QStringList availablePlaylists();
    QStringList filesInPlaylist(const QString &playlistName);
    void removeFromPlaylist(const QString &playlistName, QStringList files);
    void removeFromPlaylist(const QString &playlistName, const QString &file);
    void removePlaylist(const QString &playlistName);

    /**
     * This method export the playlist to use to the DBus interface
     * in order to keep being compliant with the mpris standard.
     */
    void setCurrentPlaylist(const QString &playlistName);

protected:
    bool sourceRequestEvent(const QString &source);
    bool updateSourceEvent(const QString &source);

private:
    class Private;
    Private *d;

};

#endif
