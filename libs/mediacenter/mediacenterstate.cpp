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

#include "mediacenterstate.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>
#include <mediacenter/medialayout.h>

using namespace MediaCenter;

MediaCenterState::MediaCenterState (QState *parent) : QState(parent),
    m_jumpToHome(new Plasma::IconWidget()),
    m_jumpToVideo(new Plasma::IconWidget()),
    m_jumpToPicture(new Plasma::IconWidget()),
    m_jumpToMusic(new Plasma::IconWidget()),    
    m_toggleControlBarAutohide(new Plasma::IconWidget())
{
}

MediaCenterState::~MediaCenterState()
{
}

void MediaCenterState::onExit(QEvent* event)
{
    Q_UNUSED(event);
}

void MediaCenterState::onEntry(QEvent* event)
{
    Q_UNUSED(event);
}
  
void MediaCenterState::connectSubComponents(QList<Plasma:: Applet*> list)
{
    Q_UNUSED(list);
}

QList<QGraphicsWidget*> MediaCenterState::mainSubComponents()
{
    QList<QGraphicsWidget*> list;
    m_toggleControlBarAutohide->setIcon("mail-attachment");
    list << m_toggleControlBarAutohide;

    m_jumpToHome->setIcon("user-home");
    list << m_jumpToHome;

    m_jumpToVideo->setIcon("folder-video");
    list << m_jumpToVideo;

    m_jumpToPicture->setIcon("folder-image");
    list << m_jumpToPicture;

    m_jumpToMusic->setIcon("folder-sound");
    list << m_jumpToMusic;    

    return list;
}

void MediaCenterState::connectMainSubComponents(QList<Plasma:: Applet*> list)
{
    foreach (Plasma::Applet *applet, list) {
        MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
        if (browser) {
        }
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(applet);
        if (player) {
        }
        MediaCenter::PlaybackControl *control = qobject_cast<MediaCenter::PlaybackControl*>(applet);
        if (control) {
            connect (m_jumpToPicture, SIGNAL(clicked()), control, SIGNAL(layoutToPictureState()));
            connect (m_jumpToVideo, SIGNAL(clicked()), control, SIGNAL(layoutToVideoState()));
            connect (m_jumpToMusic, SIGNAL(clicked()), control, SIGNAL(layoutToMusicState()));
        }
        MediaCenter::Playlist *playlist = qobject_cast<MediaCenter::Playlist*>(applet);
        if (playlist) {
        }
    }
}

void MediaCenterState::connectMediaLayout(MediaLayout* layout)
{
  //connect (m_toggleControlBarAutohide, SIGNAL(clicked()), layout, SLOT(toggleControlAutohide())); //FIXME: Where is the problem?
}

void MediaCenterState::configureMediaLayout(MediaLayout* layout)
{
    Q_UNUSED(layout);
}

void MediaCenterState::configureUIComponents(QList<Plasma:: Applet*> list)
{
    Q_UNUSED(list);
}

QList<QGraphicsWidget*> MediaCenterState::subComponents()
{
    return QList<QGraphicsWidget*>();
}

