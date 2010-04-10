/***************************************************************************
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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

#include "musicstate.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>

using namespace MediaCenter;

MusicState::MusicState (QState *parent)
    : MediaCenterState(parent),
    m_musicStateLabel(new Plasma::IconWidget()),
    m_musicTogglePlaylist(new Plasma::IconWidget())    
{
}

MusicState::~MusicState()
{
}

void MusicState::onExit(QEvent* event)
{
    Q_UNUSED(event);
}

void MusicState::onEntry(QEvent* event)
{
    Q_UNUSED(event);
    MediaCenter::Mode newState = MediaCenter::MusicMode;
    emit state(newState);
}

QList<QGraphicsWidget*> MusicState::subComponents()
{
    QList<QGraphicsWidget*> list;

    m_musicStateLabel->setText("Music State");
    list << m_musicStateLabel;
    
    m_musicTogglePlaylist->setIcon("");
    list << m_musicStateLabel;
    
    return list;
}

void MusicState::configureUIComponents(QList<Plasma:: Applet*> list)
{
      foreach (Plasma::Applet *applet, list) {
        MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
        if (browser) {
        }
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(applet);
        if (player) {
            player->setVisible(false);
        }
        MediaCenter::PlaybackControl *control = qobject_cast<MediaCenter::PlaybackControl*>(applet);
        if (control) {
        }
        MediaCenter::Playlist *playlist = qobject_cast<MediaCenter::Playlist*>(applet);
        if (playlist) {
            playlist->setVisible(true);
        }
    }
}

void MusicState::connectMediaLayout(MediaLayout *layout)
{
    Q_UNUSED(layout);
    //connect (m_musicTogglePlaylist, SIGNAL(clicked()), layout, SLOT(togglePlaylistVisible())); FIXME BUILD Error
}

void MusicState::configureMediaLayout(MediaLayout *layout)
{
    Q_UNUSED(layout);
    //layout->setPlaylistVisible(true); FIXME BUILD error
}

void MusicState::connectSubComponents(QList<Plasma:: Applet*> list)
{
    Q_UNUSED(list);
}
