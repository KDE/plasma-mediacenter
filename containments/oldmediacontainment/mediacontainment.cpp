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
#include <mediacenter/infodisplay.h>
#include <mediacenter/homeapplet.h>

#include <mediacenter/mediacenterstate.h>
#include <mediacenter/videostate.h>
#include <mediacenter/picturestate.h>
#include <mediacenter/musicstate.h>
#include <mediacenter/homestate.h>

// Qt
#include <QAction>
#include <QStateMachine>
#include <QState>

// KDE
#include <KDebug>
#include <KNotification>
#include <KLocale>

K_EXPORT_PLASMA_APPLET(mediacontainment, MediaContainment)

MediaContainment::MediaContainment(QObject *parent, const QVariantList &args) : Plasma::Containment(parent, args),
m_browser(0),
m_control(0),
m_playlist(0),
m_player(0),
m_infoDisplay(0),
m_home(0),
m_homeState(0),
m_currentState(MediaCenter::HomeMode),
m_previousState(MediaCenter::HomeMode),
m_mediaCenterState(0),
m_layout(new MediaCenter::MediaLayout(this))
{
    setContainmentType(Plasma::Containment::CustomContainment);
    setHasConfigurationInterface(true);
    setAcceptHoverEvents(true);
    setToolBox(new MediaToolBox(this));

    connect (toolBox(), SIGNAL(toggled()), m_layout, SLOT(toggleShowAllMediaApplets()));
}

MediaContainment::~MediaContainment()
{
}

void MediaContainment::startStateMachine()
{
    //Prepare StateMachine
    QStateMachine *machine = new QStateMachine(this);

    //Set up all possible states
    m_mediaCenterState = new MediaCenter::MediaCenterState(machine);
    m_homeState = new MediaCenter::HomeState(machine);
    m_videoState = new MediaCenter::VideoState(machine);
    m_pictureState = new MediaCenter::PictureState(machine);
    m_musicState = new MediaCenter::MusicState(machine);

    //Define state change buttons
    m_videoState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_pictureState);
    m_videoState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_musicState);
    m_videoState->addTransition(m_mediaCenterState, SIGNAL(homeModeRequired()), m_homeState);
    m_pictureState->addTransition(m_mediaCenterState, SIGNAL(videoModeRequired()), m_videoState);
    m_pictureState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_musicState);
    m_pictureState->addTransition(m_mediaCenterState, SIGNAL(homeModeRequired()), m_homeState);
    m_musicState->addTransition(m_mediaCenterState, SIGNAL(videoModeRequired()), m_videoState);
    m_musicState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_pictureState);
    m_musicState->addTransition(m_mediaCenterState, SIGNAL(homeModeRequired()), m_homeState);
    m_homeState->addTransition(m_mediaCenterState, SIGNAL(videoModeRequired()), m_videoState);
    m_homeState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_pictureState);
    m_homeState->addTransition(m_mediaCenterState, SIGNAL(pictureModeRequired()), m_musicState);

    //I use these signals to tell the mediacontainment to do an actual state switch
    connect(m_videoState, SIGNAL(state(const MediaCenter::Mode)), this, SLOT(switchState(const MediaCenter::Mode)));
    connect(m_pictureState, SIGNAL(state(const MediaCenter::Mode)), this, SLOT(switchState(const MediaCenter::Mode)));
    connect(m_musicState, SIGNAL(state(const MediaCenter::Mode)), this, SLOT(switchState(const MediaCenter::Mode)));
    connect(m_homeState, SIGNAL(state(const MediaCenter::Mode)), this, SLOT(switchState(const MediaCenter::Mode)));

    //Initialize pointers for each state
    m_mediaCenterState->init(m_layout,m_currentUIComponents);
    m_homeState->init(m_layout,m_currentUIComponents);
    m_videoState->init(m_layout,m_currentUIComponents);
    m_pictureState->init(m_layout,m_currentUIComponents);
    m_musicState->init(m_layout,m_currentUIComponents);

    //Connect each state
    m_mediaCenterState->initConnections();
    m_homeState->initConnections();
    m_videoState->initConnections();
    m_pictureState->initConnections();
    m_musicState->initConnections();

    //Setup and start statemachine
    machine->setInitialState(m_homeState);
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

    MediaCenter::InfoDisplay *infoDisplay = qobject_cast<MediaCenter::InfoDisplay*>(applet);
    if (infoDisplay) {
        if (m_infoDisplay) {
            KNotification::event(KNotification::Error, i18n("An information display applet for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_infoDisplay = infoDisplay;
            m_layout->setInfoDisplay(m_infoDisplay);
            m_layout->invalidate();
            m_currentUIComponents << m_infoDisplay;
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

            startStateMachine();
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

    MediaCenter::HomeApplet *home = qobject_cast<MediaCenter::HomeApplet*>(applet);
    if (home) {
        if (m_home) {
            KNotification::event(KNotification::Error, i18n("A home applet for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_home = home;
            m_layout->setHomeApplet(m_home);
            m_layout->invalidate();
            m_currentUIComponents << m_home;
        }
        return;
    }
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
    } else if (applet == m_infoDisplay) {
        m_infoDisplay = 0;
    } else if (applet == m_home) {
        m_home = 0;
    }
}

void MediaContainment::switchState(const MediaCenter::Mode &newState)
{
    //Hide the currentSubComponents
    foreach (QGraphicsWidget* w, m_visibleSubComponents) {
        w->hide();
    }

    //This creates new layouts in the controller
    m_control->resetLayout();
    m_infoDisplay->resetLayout();

    setCurrentState(newState);
    QList<QGraphicsWidget*> list;

    if (newState == MediaCenter::HomeMode) {
        list = m_homeState->subComponents();
        m_homeState->configure();
    }
    if (newState == MediaCenter::PictureMode) {
        list = m_pictureState->subComponents();
        m_pictureState->configure();
    }
    if (newState== MediaCenter::VideoMode) {
        list = m_videoState->subComponents();
        m_videoState->configure();
    }
    if (newState== MediaCenter::MusicMode) {
        list = m_musicState->subComponents();
        m_musicState->configure();
    }

    //Add and configure the MainSubcompoments
    m_visibleMainComponents = m_mediaCenterState->mainSubComponents();
    list << m_visibleMainComponents;
    m_mediaCenterState->configure();

    //Show the new subComponents
    foreach (QGraphicsWidget* w , list) {
        w->show();
    }

    //This adds the layouts to the controller
    m_control->applyLayout();
    m_infoDisplay->applyLayout();

    //Update the currently visible subComponents
    m_visibleSubComponents = list;
}

void MediaContainment::setCurrentState(const MediaCenter::Mode &newstate)
{
    m_previousState = m_currentState;
    m_currentState = newstate;
}

MediaCenter::Mode MediaContainment::currentState() const
{
    return m_currentState;
}

QList<QGraphicsWidget*> MediaContainment::currentMainComponents() const
{
    return m_visibleMainComponents;
}

void MediaContainment::addCurrentMainComponent(QGraphicsWidget* widget)
{
    m_visibleMainComponents << widget;
}

QList<QGraphicsWidget*> MediaContainment::currentSubComponents() const
{
    return m_visibleSubComponents;
}

void MediaContainment::addCurrentSubComponent(QGraphicsWidget* widget)
{
    m_visibleSubComponents << widget;
}
