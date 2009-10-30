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
m_onlyBrowser(false),
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

    MediaHandler *handler = new MediaHandler(m_browser, MediaHandler::Right);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
}

void MediaLayout::setPlaybackControl(Plasma::Applet *control)
{
    m_control = control;
    m_needLayouting << m_control;
    m_control->setZValue(1000);

    MediaHandler *handler = new MediaHandler(m_control, MediaHandler::Bottom);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
}

void MediaLayout::setPlaylist(Plasma::Applet *playlist)
{
    m_playlist = playlist;
    m_needLayouting << m_playlist;
    m_playlist->setZValue(999);

    MediaHandler *handler = new MediaHandler(m_playlist, MediaHandler::Left);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
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
    if (m_onlyBrowser) {
        m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
        m_browser->setPos(m_containment->rect().topLeft());
        m_browser->resize(m_containment->size());
        return;
    }

    m_browser->setBackgroundHints(m_browserBackgroundHints);
    m_browser->setPos(browserPreferredShowingRect().topLeft());
    m_browser->resize(browserPreferredShowingRect().size());
//    m_browser->setPos(m_browser->pos().x() - m_browser->size().width(), m_browser->pos().y());
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
    return QRectF(QPointF(-leftMargin, (m_containment->size().height() - m_browser->size().height()) / 2),
                  QSizeF(m_containment->size().width() / 3.0, m_containment->size().height() * 2 / 3.0));
}

QRectF MediaLayout::controllerPreferredShowingRect() const
{
    const int width = 64 * 4 + 256;
    const int height = 64;

    return QRectF(QPointF((m_containment->size().width() - width) / 2, 0), QSizeF(width, height));
}

QRectF MediaLayout::playlistPreferredShowingRect() const
{
    if (!m_playlist) {
        return QRectF();
    }

    qreal rightMargin;
    m_playlist->getContentsMargins(0, 0, &rightMargin, 0);
    return QRectF(QPointF(m_containment->size().width() - (m_containment->size().width() / 4.0) + rightMargin , (m_containment->size().height() - m_playlist->size().height()) / 2),
                  QSizeF(m_containment->size().width() / 4.0, m_containment->size().height() * 2 / 3.0));
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

void MediaLayout::setOnlyShowBrowser(bool set)
{
    if (m_onlyBrowser == set) {
        return;
    }

    m_onlyBrowser = set;
    if (m_browser) {
        MediaHandler *handler = MediaHandler::handlerFromApplet(m_browser);
        if (handler) {
            handler->setVisible(!m_onlyBrowser);
            if (!m_onlyBrowser) {
                connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
                connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
            } else {
                disconnect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
                disconnect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
            }

            if (m_player) {
                m_player->setVisible(!m_onlyBrowser);
            }
        }
        layoutBrowser();
    }
}

bool MediaLayout::onlyShowBrowser()
{
    return m_onlyBrowser;
}

void MediaLayout::toggleShowAllMediaApplets()
{
    // let's behave this way for now
    if (m_onlyBrowser) {
        return;
    }

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
