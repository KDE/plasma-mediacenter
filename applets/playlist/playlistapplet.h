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
#ifndef PLAYLISTAPPLET_H
#define PLAYLISTAPPLET_H

#include <mediacenter/playlist.h>
#include <mediacenter/mediacenter.h>

#include "ui_config.h"

class PlaylistWidget;
class KConfigDialog;

class PlaylistApplet : public MediaCenter::Playlist
{
    Q_OBJECT
public:
    PlaylistApplet(QObject *parent, const QVariantList &args);
    ~PlaylistApplet();

    void init();
    void createConfigurationInterface(KConfigDialog *parent);

    int length();
    QStringList medias();

protected slots:
    void loadConfiguration();
    void configAccepted();

private:
    PlaylistWidget *m_playlistWidget;
    Ui::Config configUi;

    bool m_multiplePlaylists;

};

#endif
