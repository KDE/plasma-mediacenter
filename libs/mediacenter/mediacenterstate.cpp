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
#include <mediacenter/homeapplet.h>
#include <mediacenter/private/sharedlayoutcomponentsmanager.h>


using namespace MediaCenter;

MediaCenterState::MediaCenterState (QState *parent) : QState(parent),
    m_browser(0),
    m_player(0),
    m_control(0),
    m_playlist(0),
    m_infoDisplay(0),
    m_home(0),
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

    list << SharedLayoutComponentsManager::self()->homeWidget();
    m_control->addToLayout(SharedLayoutComponentsManager::self()->homeWidget(), MediaCenter::LeftZone);

    SharedLayoutComponentsManager::self()->backgroundVideoWidget()->setVisible(false);
    list << SharedLayoutComponentsManager::self()->backgroundVideoWidget();

    SharedLayoutComponentsManager::self()->backgroundPictureWidget()->setVisible(false);
    list << SharedLayoutComponentsManager::self()->backgroundPictureWidget();

    SharedLayoutComponentsManager::self()->backgroundMusicWidget()->setVisible(false);
    list << SharedLayoutComponentsManager::self()->backgroundMusicWidget();

    if (SharedLayoutComponentsManager::self()->currentMode() == MediaCenter::HomeMode) {
        m_infoDisplay->addToLayout(SharedLayoutComponentsManager::self()->backgroundVideoWidget(), MediaCenter::LeftZone);
        m_infoDisplay->addToLayout(SharedLayoutComponentsManager::self()->backgroundPictureWidget(), MediaCenter::LeftZone);
        m_infoDisplay->addToLayout(SharedLayoutComponentsManager::self()->backgroundMusicWidget(), MediaCenter::LeftZone);
    } else {
        m_control->addToLayout(SharedLayoutComponentsManager::self()->backgroundVideoWidget(), MediaCenter::LeftZone);
        m_control->addToLayout(SharedLayoutComponentsManager::self()->backgroundPictureWidget(), MediaCenter::LeftZone);
        m_control->addToLayout(SharedLayoutComponentsManager::self()->backgroundMusicWidget(), MediaCenter::LeftZone);
    }

    list << SharedLayoutComponentsManager::self()->barAutohideControlWidget();
    m_control->addToLayout(SharedLayoutComponentsManager::self()->barAutohideControlWidget(), MediaCenter::RightZone);

    return list;
}

void MediaCenterState::initConnections()
{
    connect (SharedLayoutComponentsManager::self()->barAutohideControlWidget(), SIGNAL(clicked()), m_layout, SLOT(toggleControlAutohide()));
    connect (SharedLayoutComponentsManager::self()->homeWidget(), SIGNAL(clicked()), this, SIGNAL(homeModeRequired()));

    connect (m_browser, SIGNAL(browsingModeChanged(MediaCenter::Mode)), this, SLOT(modeChangeRequired(MediaCenter::Mode)));

    connect (SharedLayoutComponentsManager::self()->backgroundMusicWidget(), SIGNAL (clicked()), this, SIGNAL(musicModeRequired()));
    connect (SharedLayoutComponentsManager::self()->backgroundVideoWidget(), SIGNAL (clicked()), this, SIGNAL(videoModeRequired()));
    connect (SharedLayoutComponentsManager::self()->backgroundPictureWidget(), SIGNAL (clicked()), this, SIGNAL(pictureModeRequired()));
}

void MediaCenterState::modeChangeRequired(MediaCenter::Mode mode)
{
    switch (mode) {
    case MediaCenter::PictureMode:
        emit pictureModeRequired();
        break;
    case MediaCenter::VideoMode:
        emit videoModeRequired();
        break;
    case MediaCenter::MusicMode:
        emit musicModeRequired();
        break;
    default:
        ;
    }
}

void MediaCenterState::configure()
{}

QList<QGraphicsWidget*> MediaCenterState::subComponents() const
{
    return QList<QGraphicsWidget*>();
}

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
        MediaCenter::HomeApplet *homeApplet = qobject_cast<MediaCenter::HomeApplet*>(applet);
        if (homeApplet) {
            m_home = homeApplet;
        }
    }
    m_layout = layout;
}

void MediaCenter::MediaCenterState::showBackgroundStates()
{
    //Hide all background icons
    SharedLayoutComponentsManager::self()->backgroundMusicWidget()->setVisible(false);
    SharedLayoutComponentsManager::self()->backgroundPictureWidget()->setVisible(false);
    SharedLayoutComponentsManager::self()->backgroundVideoWidget()->setVisible(false);

    //Unhide icons for the active background states
    if (SharedLayoutComponentsManager::self()->isBackgroundMusicMode()) {
        SharedLayoutComponentsManager::self()->backgroundMusicWidget()->setVisible(true);
    }
    if (SharedLayoutComponentsManager::self()->isBackgroundPictureMode()) {
        SharedLayoutComponentsManager::self()->backgroundPictureWidget()->setVisible(true);
    }
    if (SharedLayoutComponentsManager::self()->isBackgroundVideoMode()) {
        SharedLayoutComponentsManager::self()->backgroundVideoWidget()->setVisible(true);
    }
}

void MediaCenter::MediaCenterState::onPlaybackStateChanged(const MediaCenter::PlaybackState &state)
{
    Q_UNUSED(state);
}

void MediaCenterState::slotSetCurrentState(const MediaCenter::Mode& mode)
{
    SharedLayoutComponentsManager::self()->setCurrentMode(mode);
}

