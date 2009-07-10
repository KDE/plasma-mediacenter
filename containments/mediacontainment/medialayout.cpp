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

#include <Plasma/Containment>
#include <Plasma/Applet>
#include <Plasma/Animator>

MediaLayout::MediaLayout(Plasma::Containment *parent) : QObject(parent),
m_containment(parent),
m_browser(0),
m_control(0),
m_playlist(0)
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

    MediaHandler *handler = new MediaHandler(m_browser, MediaHandler::Right);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
}

void MediaLayout::setPlaybackControl(Plasma::Applet *control)
{
    m_control = control;
    m_needLayouting << m_control;

    MediaHandler *handler = new MediaHandler(m_control, MediaHandler::Bottom);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
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
}

void MediaLayout::layoutBrowser()
{
    m_browser->setPos(browserPreferredShowingRect().topLeft());
    m_browser->resize(browserPreferredShowingRect().size());
//    m_browser->setPos(m_browser->pos().x() - m_browser->size().width(), m_browser->pos().y());
}

void MediaLayout::layoutControl()
{
    m_control->setPos(controllerPreferredShowingRect().topLeft());
    m_control->resize(controllerPreferredShowingRect().size());
}

void MediaLayout::layoutPlaylist() {
    m_playlist->setPos(playlistPreferredShowingRect().topLeft());
    m_playlist->resize(playlistPreferredShowingRect().size());
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
    return QRectF(QPointF(0, (m_containment->size().height() - m_browser->size().height()) / 2),
                  QSizeF(m_containment->size().width() / 3.0, m_containment->size().height() * 2 / 3.0));
}

QRectF MediaLayout::controllerPreferredShowingRect() const
{
    const int width = 64 * 4;
    const int height = 64;

    return QRectF(QPointF((m_containment->size().width() - width) / 2, 0), QSizeF(width, height));
}

QRectF MediaLayout::playlistPreferredShowingRect() const
{
    return QRectF(QPointF(m_containment->size().width(), (m_containment->size().height() - m_playlist->size().height()) / 2),
                  QSizeF(m_containment->size().width() / 3.0, m_containment->size().height() * 2 / 3.0));
}

void MediaLayout::animateHidingApplet(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(m_browser->rect().x() - m_browser->size().width(),
                                                                                              browserPreferredShowingRect().y()));
    } else if (applet == m_control) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(controllerPreferredShowingRect().x(),
                                                                                              m_control->rect().y() - m_control->size().height()));
    } else if (applet == m_playlist) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(playlistPreferredShowingRect().x() + m_playlist->size().width(),
                                                                                              m_playlist->rect().x()));
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
