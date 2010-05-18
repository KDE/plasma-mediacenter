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
#include <mediacenter/infodisplay.h>
#include <mediacenter/medialayout.h>


using namespace MediaCenter;

//Static variables initialization
Plasma::IconWidget *MediaCenterState::s_jumpToHome = new Plasma::IconWidget();
Plasma::IconWidget *MediaCenterState::s_toggleControlBarAutohide = new Plasma::IconWidget();
Plasma::IconWidget *MediaCenterState::s_browserGoPrevious = new Plasma::IconWidget();

Plasma::IconWidget *MediaCenterState::s_backgroundMusic = new Plasma::IconWidget();
Plasma::IconWidget *MediaCenterState::s_backgroundPicture = new Plasma::IconWidget();
Plasma::IconWidget *MediaCenterState::s_backgroundVideo = new Plasma::IconWidget();

bool MediaCenterState::s_backgroundPictureMode = false;
bool MediaCenterState::s_backgroundVideoMode = false;
bool MediaCenterState::s_backgroundMusicMode = false;

MediaCenter::Mode MediaCenterState::s_currentState = HomeMode;

MediaCenterState::MediaCenterState (QState *parent) : QState(parent),
    m_browser(0),
    m_player(0),
    m_control(0),
    m_playlist(0),
    m_infoDisplay(0),
    m_layout(0)
{
    connect(this, SIGNAL(state(const MediaCenter::Mode)), this, SLOT(slotSetCurrentState(const MediaCenter::Mode)));
}

MediaCenterState::~MediaCenterState()
{}

void MediaCenterState::onExit(QEvent* event)
{
    Q_UNUSED(event);
}

void MediaCenterState::onEntry(QEvent* event)
{
    Q_UNUSED(event);
}

QList<QGraphicsWidget*> MediaCenterState::mainSubComponents() const
{
    QList<QGraphicsWidget*> list;

    s_jumpToHome->setIcon("user-home");
    list << s_jumpToHome;
    m_control->addToLayout(s_jumpToHome, MediaCenter::ControlLeft);

    s_browserGoPrevious->setIcon("go-previous-view");
    list << s_browserGoPrevious;
    m_control->addToLayout(s_browserGoPrevious, MediaCenter::ControlLeft);

    s_backgroundVideo->setIcon("folder-video");
    s_backgroundVideo->setVisible(false);
    list << s_backgroundVideo;

    s_backgroundPicture->setIcon("folder-image");
    s_backgroundPicture->setVisible(false);
    list << s_backgroundPicture;

    s_backgroundMusic->setIcon("folder-sound");
    s_backgroundMusic->setVisible(false);
    list << s_backgroundMusic;

    if (s_currentState == MediaCenter::HomeMode) {
        m_infoDisplay->addToLayout(s_backgroundVideo, MediaCenter::ControlLeft);
        m_infoDisplay->addToLayout(s_backgroundPicture, MediaCenter::ControlLeft);
        m_infoDisplay->addToLayout(s_backgroundMusic, MediaCenter::ControlLeft);
    } else {
        m_control->addToLayout(s_backgroundVideo, MediaCenter::ControlRight);
        m_control->addToLayout(s_backgroundPicture, MediaCenter::ControlRight);
        m_control->addToLayout(s_backgroundMusic, MediaCenter::ControlRight);
    }

    s_toggleControlBarAutohide->setIcon("mail-attachment");
    list << s_toggleControlBarAutohide;
    m_control->addToLayout(s_toggleControlBarAutohide, MediaCenter::ControlBottom);

    return list;
}

void MediaCenterState::initConnections()
{
    connect (s_browserGoPrevious, SIGNAL(clicked()), m_browser, SIGNAL(goPrevious()));
    connect (s_toggleControlBarAutohide, SIGNAL(clicked()), m_layout, SLOT(toggleControlAutohide()));
    connect (s_jumpToHome, SIGNAL(clicked()), this, SIGNAL(layoutToHomeState()));

    connect (m_browser, SIGNAL(pictureDataEngine()), this, SIGNAL(layoutToPictureState()));
    connect (m_browser, SIGNAL(videoDataEngine()), this, SIGNAL(layoutToVideoState()));
    connect (m_browser, SIGNAL(musicDataEngine()), this, SIGNAL(layoutToMusicState()));

    connect (s_backgroundMusic, SIGNAL (clicked()), this, SIGNAL(layoutToMusicState()));
    connect (s_backgroundVideo, SIGNAL (clicked()), this, SIGNAL(layoutToVideoState()));
    connect (s_backgroundPicture, SIGNAL (clicked()), this, SIGNAL(layoutToPictureState()));
}

void MediaCenterState::configure()
{}

QList<QGraphicsWidget*> MediaCenterState::subComponents() const
{}

void MediaCenterState::init(MediaCenter::MediaLayout* &layout, QList< Plasma::Applet* > &list)
{
    foreach (Plasma::Applet *applet, list) {
        MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
        if (browser) {
            m_browser = browser;
        }
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(applet);
        if (player) {
            m_player = player;
        }
        MediaCenter::PlaybackControl *control = qobject_cast<MediaCenter::PlaybackControl*>(applet);
        if (control) {
            m_control = control;
        }
        MediaCenter::Playlist *playlist = qobject_cast<MediaCenter::Playlist*>(applet);
        if (playlist) {
            m_playlist = playlist;
        }
        MediaCenter::InfoDisplay *infoDisplay = qobject_cast<MediaCenter::InfoDisplay*>(applet);
        if (infoDisplay) {
            m_infoDisplay = infoDisplay;
        }
    }
    m_layout = layout;
}

void MediaCenter::MediaCenterState::showBackgroundStates()
{
    //Hide all background icons
    s_backgroundMusic->setVisible(false);
    s_backgroundPicture->setVisible(false);
    s_backgroundVideo->setVisible(false);

    //Unhide icons for the active background states
    if (s_backgroundMusicMode) {
        s_backgroundMusic->setVisible(true);
    }
    if (s_backgroundPictureMode) {
        s_backgroundPicture->setVisible(true);
    }
    if (s_backgroundVideoMode) {
        s_backgroundVideo->setVisible(true);
    }
}

void MediaCenter::MediaCenterState::onPlaybackStateChanged(const MediaCenter::PlaybackState &state)
{
    Q_UNUSED(state);
}

void MediaCenterState::slotSetCurrentState(const MediaCenter::Mode& mode)
{
    s_currentState = mode;
}

