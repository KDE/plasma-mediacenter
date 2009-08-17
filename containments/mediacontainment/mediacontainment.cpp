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
#include "mediacontainment.h"
#include "medianotificationwidget.h"
#include "medialayout.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>

// Qt
#include <QAction>

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
m_layout(new MediaLayout(this))
{
    setContainmentType(Plasma::Containment::CustomContainment);
    setHasConfigurationInterface(true);
    setAcceptHoverEvents(true);
}

MediaContainment::~MediaContainment()
{}

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

            if (m_player) {
                initControls();
            }
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

            if (m_player) {
                m_player->enqueue(m_playlist->medias());
            }
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

            m_layout->setOnlyShowBrowser(!m_player->isActive());
            connect (m_player, SIGNAL(activeStateChanged(bool)), this, SLOT(slotPlayerActive(bool)));
            if (m_control) {
                initControls();
            }

            if (m_playlist) {
                m_player->enqueue(m_playlist->medias());
            }
        }
        return;
    }

}

void MediaContainment::initControls()
{
    connect (m_control, SIGNAL(playPauseRequest()), m_player, SLOT(playPause()));
    connect (m_control, SIGNAL(seekRequest(int)), m_player, SLOT(seek(int)));
    connect (m_control, SIGNAL(volumeLevelChangeRequest(qreal)), m_player, SLOT(setVolume(qreal)));
    connect (m_control, SIGNAL(stopRequest()), m_player, SLOT(stop()));
    connect (m_control, SIGNAL(mediaSkipBackwardRequest()), m_player, SLOT(skipBackward()));
    connect (m_control, SIGNAL(mediaSkipForwardRequest()), m_player, SLOT(skipForward()));
    connect (m_player, SIGNAL(mediaReceived(QStringList)), m_playlist, SLOT(appendMedia(QStringList)));
    connect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueue(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playMedia(const MediaCenter::Media&)));
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

void MediaContainment::slotPlayerActive(bool active)
{
    m_layout->setOnlyShowBrowser(!active);
}
