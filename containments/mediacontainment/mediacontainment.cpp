/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "mediacontainment.h"
#include "medianotificationwidget.h"
#include "mediatoolbox.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>
#include <mediacenter/medialayout.h>

#include <mediacenter/mediacenterstate.h>
#include <mediacenter/videostate.h>
#include <mediacenter/picturestate.h>
#include <mediacenter/musicstate.h>

// Qt
#include <QAction>
#include <QStateMachine>
#include <QState>

// KDE
#include <KDebug>
#include <KNotification>
#include <KLocale>

static const int BROWSER_WIDTH = 300;
static const int BROWSER_HEIGHT = 100;


K_EXPORT_PLASMA_APPLET(mediacontainment, MediaContainment)

MediaContainment::MediaContainment(QObject *parent, const QVariantList &args) : Plasma::Containment(parent, args),
m_browser(0),
m_control(0),
m_playlist(0),
m_player(0),
m_videoState(0),
m_currentState(MediaCenter::PictureMode),
m_pictureState(0),
m_previousState(MediaCenter::PictureMode),
m_mediaCenterState(0),
m_musicIsPlaying(false),
m_layout(new MediaLayout(this))
{
    setContainmentType(Plasma::Containment::CustomContainment);
    setHasConfigurationInterface(true);
    setAcceptHoverEvents(true);
    setToolBox(new MediaToolBox(this));

    connect (toolBox(), SIGNAL(toggled()), m_layout, SLOT(toggleShowAllMediaApplets()));
}

MediaContainment::~MediaContainment()
{}

void MediaContainment::startStateMachine()
{
    //Prepare StateMachine
    QStateMachine *machine = new QStateMachine();

    //Set up all possible states
    m_mediaCenterState = new MediaCenter::MediaCenterState();
    m_videoState = new MediaCenter::VideoState(machine);
    m_pictureState = new MediaCenter::PictureState(machine);
    m_musicState = new MediaCenter::MusicState(machine);    

    //Define state change buttons
    m_videoState->addTransition(m_control, SIGNAL(layoutToPictureState()), m_pictureState);
    m_videoState->addTransition(m_control, SIGNAL(layoutToMusicState()), m_musicState);
    m_pictureState->addTransition(m_control, SIGNAL(layoutToVideoState()), m_videoState);
    m_pictureState->addTransition(m_control, SIGNAL(layoutToMusicState()), m_musicState);
    m_musicState->addTransition(m_control, SIGNAL(layoutToVideoState()), m_videoState);
    m_musicState->addTransition(m_control, SIGNAL(layoutToPictureState()), m_pictureState);    

    //I use these signals to tell the mediacontainment to do an actual state switch
    connect (m_videoState, SIGNAL(state(MediaCenter::Mode)), this, SLOT(switchState(MediaCenter::Mode)));
    connect (m_pictureState, SIGNAL(state(MediaCenter::Mode)), this, SLOT(switchState(MediaCenter::Mode)));
    connect (m_musicState, SIGNAL(state(MediaCenter::Mode)), this, SLOT(switchState(MediaCenter::Mode)));

    //connections for MainComponents
    m_mediaCenterState->connectMainSubComponents(m_currentUIComponents);
    //m_mediaCenterState->connectMediaLayout(m_layout); FIXME build error
    
    //connections of all subcomponents for all states
    m_pictureState->connectSubComponents(m_currentUIComponents);
    m_videoState->connectSubComponents(m_currentUIComponents);
    m_musicState->connectSubComponents(m_currentUIComponents);    

    //Setup and start statemachine
    machine->setInitialState(m_videoState);
    machine->start();
}

QList<QAction*> MediaContainment::contextualActions()
{
    QList<QAction*> actions;

    actions << action("add widgets");
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    actions << separator;
    actions << action("configure");

    return actions;
}

void MediaContainment::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::StartupCompletedConstraint) {
        connect (this, SIGNAL(appletAdded(Plasma::Applet*, const QPointF &)), SLOT(slotAppletAdded(Plasma::Applet*, const QPointF &)));
        connect (this, SIGNAL(appletRemoved(Plasma::Applet*)), SLOT(slotAppletRemoved(Plasma::Applet*)));
    }
}

void MediaContainment::slotAppletAdded(Plasma::Applet *applet, const QPointF &pos)
{
    Q_UNUSED(pos)

    // browser check
    MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
    if (browser) {
        if (m_browser) {
            KNotification::event(KNotification::Error, i18n("A browser for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_browser = browser;
            m_layout->setBrowser(m_browser);
            m_layout->invalidate();
	    m_currentUIComponents << m_browser;
        }
        return;
    }

    MediaCenter::PlaybackControl *control = qobject_cast<MediaCenter::PlaybackControl*>(applet);
    if (control) {
        if (m_control) {
            KNotification::event(KNotification::Error, i18n("A playback control for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_control = control;
            m_layout->setPlaybackControl(m_control);
            m_layout->invalidate();
            m_currentUIComponents << m_control;

            initControls();

            //This is the last applet added, so start the machine
            m_layout->setControlAutohide(false); //TODO Put this somewhere else
            startStateMachine();

            //FIXME The following is a hack to get PMC into a good initial state
            MediaCenter::Mode state1 = MediaCenter::PictureMode;
            MediaCenter::Mode state2 = MediaCenter::VideoMode;
            MediaCenter::Mode state3 = MediaCenter::MusicMode;
            switchState(state1);
            switchState(state2);
            switchState(state3);
        }
        return;
    }

    MediaCenter::Playlist *playlist = qobject_cast<MediaCenter::Playlist*>(applet);
    if (playlist) {
        if (m_playlist) {
            KNotification::event(KNotification::Error, i18n("A playlist applet for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_playlist = playlist;
            m_layout->setPlaylist(m_playlist);
            m_layout->invalidate();
	    m_currentUIComponents << m_playlist;
        }
        return;
    }

    MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(applet);
    if (player) {
        if (m_player) {
            KNotification::event(KNotification::Error, i18n("A media player applet for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_player = player;
            m_layout->setPlayer(m_player);
            m_layout->invalidate();
            m_currentUIComponents << m_player;
        }
        return;
    }
}

void MediaContainment::initControls()
{
    if (!m_player) {
        return;
    }

    if (!m_control) {
        return;
    }
    kDebug() << "Init controls";
    connect(m_control, SIGNAL(toggleControlAutohide()), m_layout, SLOT(toggleControlAutohide()));

    connect(m_control, SIGNAL(browserGoPrevious()), m_browser, SIGNAL(goPrevious()));
    connect(m_control, SIGNAL(togglePlaylistVisible()), m_layout, SLOT(togglePlaylistVisible()));
    connect (m_control, SIGNAL(playPauseRequest()), m_player, SLOT(playPause()));
    connect (m_control, SIGNAL(seekRequest(int)), m_player, SLOT(seek(int)));
    connect (m_control, SIGNAL(volumeLevelChangeRequest(qreal)), m_player, SLOT(setVolume(qreal)));
    connect (m_control, SIGNAL(stopRequest()), m_player, SLOT(stop()));
    connect (m_control, SIGNAL(mediaSkipBackwardRequest()), m_player, SLOT(skipBackward()));
    connect (m_control, SIGNAL(mediaSkipForwardRequest()), m_player, SLOT(skipForward()));
    connect (m_player, SIGNAL(playbackStateChanged(MediaCenter::PlaybackState)), m_control, SLOT(playbackStateChanged(MediaCenter::PlaybackState)));
    m_control->setMediaObject(m_player->mediaObject());
}

void MediaContainment::slotAppletRemoved(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        m_browser = 0;
    } else if (applet == m_player) {
        m_player = 0;
    } else if (applet == m_playlist) {
        m_playlist = 0;
    } else if (applet == m_control) {
        m_control = 0;
    }
}

void MediaContainment::switchState(MediaCenter::Mode newState)
{
    setCurrentState(newState);
    QList<QGraphicsWidget*> list;

    if (newState == MediaCenter::PictureMode) {
        list = m_pictureState->subComponents();
        m_pictureState->configureUIComponents(m_currentUIComponents); //this also configures the UIComponents and sets connections
    }
    if (newState== MediaCenter::VideoMode) {
        list = m_videoState->subComponents();
        m_videoState->configureUIComponents(m_currentUIComponents); //this also configures the UIComponents and sets connections
    }
    if (newState== MediaCenter::MusicMode) {
        list = m_musicState->subComponents();
        m_musicState->configureUIComponents(m_currentUIComponents); //this also configures the UIComponents and sets connections
    }

    //Add and configure the MainSubcompoments
    if (m_visibleMainComponents.isEmpty()) { //Do all this only add initialization
        m_visibleMainComponents = m_mediaCenterState->mainSubComponents();
        m_control->addToLayout(m_visibleMainComponents);
	//m_mediaCenterState->configureMediaLayout(m_layout);  FIXME build error
    }

    //Hide the currentSubComponents
    foreach (QGraphicsWidget *w, m_visibleSubComponents) {
        w->hide();
    }

    //Add to the applet
    m_control->addToLayout(list);

    //Show the the new subComponents
    foreach (QGraphicsWidget *w, list) {
        w->show();
    }

    //Update the currently visible subComponents
    m_visibleSubComponents.clear();
    m_visibleSubComponents = list;
}

void MediaContainment::setCurrentState(MediaCenter::Mode newstate)
{
    m_previousState = m_currentState;
    m_currentState = newstate;
}

MediaCenter::Mode MediaContainment::currentState()
{
    return m_currentState;
}

QList<QGraphicsWidget*> MediaContainment::currentMainComponents()
{
    return m_visibleMainComponents;
}

void MediaContainment::addCurrentMainComponent(QGraphicsWidget* c)
{
    m_visibleMainComponents << c;
}

QList<QGraphicsWidget*> MediaContainment::currentSubComponents()
{
    return m_visibleSubComponents;
}

void MediaContainment::addCurrentSubComponent(QGraphicsWidget* c)
{
    m_visibleSubComponents << c;
}

