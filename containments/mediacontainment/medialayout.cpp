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
#include "medialayout.h"
#include "mediahandler.h"

#include <QGraphicsItem>

#include <KDebug>

#include <Plasma/Containment>
#include <Plasma/Animator>

MediaLayout::MediaLayout(Plasma::Containment *parent) : QObject(parent),
m_containment(parent),
m_browser(0),
m_control(0),
m_playlist(0),
m_player(0),
m_playlistHandler(0),
m_controlHandler(0),
m_playlistVisible(false),
m_controlAutohide(false),
m_showAll(false)
{
    m_containment->installEventFilter(this);
}

MediaLayout::~MediaLayout()
{
}

void MediaLayout::setBrowser(Plasma::Applet *browser)
{
    m_browser = browser;
    m_needLayouting << m_browser;

    m_browserBackgroundHints = m_browser->backgroundHints();
}

void MediaLayout::setPlaybackControl(Plasma::Applet *control)
{
    m_control = control;
    m_needLayouting << m_control;
    m_control->setZValue(1000);

    MediaHandler *handler = new MediaHandler(m_control, MediaHandler::Bottom);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));

    m_controlHandler = handler;
}

void MediaLayout::setPlaylist(Plasma::Applet *playlist)
{
    m_playlist = playlist;
    m_needLayouting << m_playlist;
    m_playlist->setZValue(999);

    MediaHandler *handler = new MediaHandler(m_playlist, MediaHandler::Left);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));

    m_playlistHandler = handler;
}

void MediaLayout::setPlayer(Plasma::Applet *player)
{
    m_player = player;
    m_player->setZValue(-50);
    m_needLayouting << m_player;
}

void MediaLayout::invalidate()
{
    foreach (Plasma::Applet *applet, m_needLayouting) {
        if (applet == m_browser) {
            layoutBrowser();
        } else if (applet == m_control) {
            layoutControl();
        } else if (applet == m_playlist) {
            layoutPlaylist();
        } else if (applet == m_player) {
            layoutPlayer();
	}
        m_needLayouting.removeAll(applet);
     }
}

void MediaLayout::doCompleteLayout()
{
    if (m_browser) {
        layoutBrowser();
    }
    if (m_control) {
        layoutControl();
    }
    if (m_playlist) {
        layoutPlaylist();
    }
    if (m_player) {
        layoutPlayer();
    }
}

void MediaLayout::layoutBrowser()
{
      m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
      m_browser->setPos(m_containment->rect().left(), m_containment->rect().top() +
                        controllerPreferredShowingRect().size().height());
      m_browser->resize(m_containment->size().width(), m_containment->size().height() -
                        controllerPreferredShowingRect().size().height());
}

void MediaLayout::layoutControl()
{
    m_control->setPos(QPointF(controllerPreferredShowingRect().left(),
            controllerPreferredShowingRect().top() - controllerPreferredShowingRect().height()));
    m_control->resize(controllerPreferredShowingRect().size());
}

void MediaLayout::layoutPlaylist()
{
    m_playlist->setPos(playlistPreferredShowingRect().topRight());
    m_playlist->resize(playlistPreferredShowingRect().size());
}

void MediaLayout::layoutPlayer()
{
    m_player->setPos(0, 0);
    m_player->resize(m_containment->size());
}

bool MediaLayout::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_containment && e->type() == QEvent::GraphicsSceneResize) {
        doCompleteLayout();
    }
    return false;
}

QRectF MediaLayout::browserPreferredShowingRect() const
{
    if (!m_browser) {
        return QRectF();
    }

    qreal leftMargin;
    m_browser->getContentsMargins(&leftMargin, 0, 0, 0);
    return QRectF(QPointF(-leftMargin, (m_containment->size().height() - controllerPreferredShowingRect().size().height())),
                  QSizeF(m_containment->size().width(), m_containment->size().height() -
                  controllerPreferredShowingRect().size().height()));
}

QRectF MediaLayout::controllerPreferredShowingRect() const
{
    const int width = m_containment->size().width();
    const int height = 60;

    return QRectF(QPointF((m_containment->size().width() - width) / 2, 0), QSizeF(width, height));
}

QRectF MediaLayout::playlistPreferredShowingRect() const
{
    if (!m_playlist) {
        return QRectF();
    }

    qreal rightMargin;
    m_playlist->getContentsMargins(0, 0, &rightMargin, 0);
    return QRectF(QPointF(m_containment->size().width() - (m_containment->size().width() / 4.0) + rightMargin,
                          (controllerPreferredShowingRect().height())),
                           QSizeF(m_containment->size().width() / 4.0, m_containment->size().height() -
                           controllerPreferredShowingRect().size().height()));
}

void MediaLayout::animateHidingApplet(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(-m_browser->size().width(),
                                                                                              browserPreferredShowingRect().y()));
    } else if (applet == m_control) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(controllerPreferredShowingRect().x(),
                                                                                              m_control->rect().y() - m_control->size().height()));
    } else if (applet == m_playlist) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(m_containment->size().width(),
                                                                                              playlistPreferredShowingRect().y()));
    }
}

void MediaLayout::animateShowingApplet(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, browserPreferredShowingRect().topLeft().toPoint());

    } else if (applet == m_control) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, controllerPreferredShowingRect().topLeft().toPoint());

    } else if (applet == m_playlist) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, playlistPreferredShowingRect().topLeft().toPoint());
    }
}

void MediaLayout::setEnableGlowHandler(QGraphicsItem *item, bool set)
{
    Plasma::Applet *applet = qgraphicsitem_cast<Plasma::Applet*>(item);
    if (!applet) {
        return;
    }

    MediaHandler *handler = MediaHandler::handlerFromApplet(applet);
    if (!handler) {
        return;
    }
    handler->setStopHoverEvents(!set);
    kDebug() << "set stop hovers to" << !set;
}

void MediaLayout::toggleShowAllMediaApplets()
{
    kDebug() << "setting show all to" << !m_showAll;

    if (!m_showAll) {
        if (m_browser) {
	    animateShowingApplet(m_browser);
	    setEnableGlowHandler(m_browser, false);
        }
        if (m_playlist) {
	    animateShowingApplet(m_playlist);
	    setEnableGlowHandler(m_playlist, false);
        }
        if (m_control) {
	    animateShowingApplet(m_control);
	    setEnableGlowHandler(m_control, false);
        }
        m_showAll = true;
    } else {
        if (m_browser) {
            animateHidingApplet(m_browser);
            setEnableGlowHandler(m_browser, true);
        }
        if (m_playlist) {
            animateHidingApplet(m_playlist);
            setEnableGlowHandler(m_playlist, true);
        }
        if (m_control) {
            animateHidingApplet(m_control);
            setEnableGlowHandler(m_control, true);
        }
        m_showAll = false;
    }
}

void MediaLayout::togglePlaylistVisible()
{
    if (m_playlistVisible == true) {
        animateHidingApplet(m_playlist);
        m_playlistVisible = false;
        connect (m_playlistHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_playlistHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    } else {
        animateShowingApplet(m_playlist);
        m_playlistVisible = true;
        disconnect (m_playlistHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_playlistHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    }
}

void MediaLayout::toggleControlAutohide()
{
    if (m_controlAutohide == true) {
        animateHidingApplet(m_control);
        m_controlAutohide = false;
        connect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    } else {
        animateShowingApplet(m_control);
        m_controlAutohide = true;
        disconnect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    }
}

void MediaLayout::setControlAutohide(bool set) {
    m_controlAutohide = set;
    toggleControlAutohide();
}

bool MediaLayout::playlistVisible() {
    return m_playlistVisible;
}

void MediaLayout::setPlaylistVisible (bool set) {
    if (m_playlistVisible == set) {
        return;
    }
    m_playlistVisible = !set;
    togglePlaylistVisible();
}