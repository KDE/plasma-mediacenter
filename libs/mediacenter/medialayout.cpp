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
#include "private/mediahandler_p.h"

#include "player.h"

#include <QGraphicsItem>

#include <KDebug>

#include <Plasma/Containment>
#include <Plasma/Animator>

using namespace MediaCenter;

MediaLayout::MediaLayout(Plasma::Containment *parent) : QObject(parent),
m_containment(parent),
m_browser(0),
m_control(0),
m_playlist(0),
m_player(0),
m_infoDisplay(0),
m_showAll(false),
m_playlistVisible(false),
m_controlAutohide(false),
m_playlistHandler(0),
m_controlHandler(0),
m_infoDisplayHandler(0),
m_infoDisplayOnly(false),
m_infoDisplayMode(MediaCenter::InfoDisplayBottom)

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

void MediaLayout::setInfoDisplay(Plasma::Applet *infoDisplay)
{
    m_infoDisplay = infoDisplay;
    m_needLayouting << m_infoDisplay;
    m_infoDisplay->setZValue(2000);

    MediaHandler *handler = new MediaHandler(m_infoDisplay, MediaHandler::Top);
    connect (handler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
    connect (handler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));

    m_infoDisplayHandler = handler;
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
    if (m_needLayouting.isEmpty()) {
      //FIXME: part of my repainting hack for floating mode
        doCompleteLayout();
    }
    foreach (Plasma::Applet *applet, m_needLayouting) {
        if (applet == m_browser) {
            layoutBrowser();
        } else if (applet == m_control) {
            layoutControl();
        } else if (applet == m_playlist) {
            layoutPlaylist();
        } else if (applet == m_player) {
            layoutPlayer();
        } else if (applet == m_infoDisplay) {
            layoutInfoDisplay();
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
    if (m_infoDisplay) {
        layoutInfoDisplay();
    }
}

void MediaLayout::layoutBrowser()
{
    if (m_playlistVisible) {
        if (!m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top());
            m_browser->resize(m_containment->size().width() - playlistPreferredShowingRect().size().width(), m_containment->size().height());
        }
        if (m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            m_browser->resize(m_containment->size().width() - playlistPreferredShowingRect().size().width(), m_containment->size().height() -
                              controllerPreferredShowingRect().size().height() - infoDisplayPreferredShowingRect().size().height());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            m_browser->resize(infoDisplayPreferredShowingRect().left(), m_containment->size().height() -
                              controllerPreferredShowingRect().size().height());
        }
    }
    if (!m_playlistVisible) {
        if (!m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top());
            m_browser->resize(m_containment->size().width(), m_containment->size().height());
        }
        if (m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            m_browser->resize(m_containment->size().width(), m_containment->size().height() -
                            controllerPreferredShowingRect().size().height() - infoDisplayPreferredShowingRect().size().height());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            m_browser->setPos(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            m_browser->resize(infoDisplayPreferredShowingRect().left(), m_containment->size().height() -
                              controllerPreferredShowingRect().size().height());
        }
    }
    if (m_infoDisplayOnly) {
        m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
        m_browser->setPos(m_containment->rect().left(), m_containment->rect().top());
        m_browser->resize(m_containment->size().width(), m_containment->size().height() - infoDisplayPreferredShowingRect().size().height());
    }
}

void MediaLayout::layoutControl()
{
    if (!m_controlAutohide) {
        m_control->setPos(QPointF(m_containment->rect().left(), m_containment->rect().top() -
                            controllerPreferredShowingRect().height()));
        m_control->resize(controllerPreferredShowingRect().size());
    }
    if (m_controlAutohide) {
        m_control->setPos(QPointF(m_containment->rect().left(), m_containment->rect().top()));
        m_control->resize(controllerPreferredShowingRect().size());
    }
}

void MediaLayout::layoutInfoDisplay()
{
    m_infoDisplay->setMinimumWidth(300);

    if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
        if (!m_controlAutohide) {
            m_infoDisplay->setPos(QPointF(m_containment->rect().left(), m_containment->rect().bottom()));
            m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
        }
        if (m_controlAutohide) {
            m_infoDisplay->setPos(QPointF(m_containment->rect().left(), m_containment->rect().bottom() -
                                infoDisplayPreferredShowingRect().height()));
            m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
        }
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
        m_infoDisplay->setPos(QPointF(infoDisplayPreferredShowingRect().left(), infoDisplayPreferredShowingRect().top()));
        m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
    }
}

//FIXME there is an error somewhere in here :-/ When the bars are hidden, the playlist
//does not show/hide correctly
void MediaLayout::layoutPlaylist()
{
    if (m_playlistVisible) {
        if (!m_controlAutohide) {
            m_playlist->setPos(playlistPreferredShowingRect().left(),0);
            m_playlist->resize(playlistPreferredShowingRect().size());
        }
        if (m_controlAutohide) {
            m_playlist->setPos(playlistPreferredShowingRect().left(), controllerPreferredShowingRect().height());
            m_playlist->resize(playlistPreferredShowingRect().size().width(), playlistPreferredShowingRect().size().height() -
                         infoDisplayPreferredShowingRect().size().height() - controllerPreferredShowingRect().size().height());
        }
    }
    if (!m_playlistVisible) {
        if (!m_controlAutohide) {
            m_playlist->setPos(playlistPreferredShowingRect().right(), 0);
            m_playlist->resize(playlistPreferredShowingRect().size());
        }
        if (m_controlAutohide) {
            m_playlist->setPos(playlistPreferredShowingRect().right(),0);
            m_playlist->resize(playlistPreferredShowingRect().size());
        }
    }
}

void MediaLayout::layoutPlayer()
{
    const qreal MARGINFACTOR = 0.2;

    m_player->setMinimumSize(50,50);

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(m_player);
        QRectF pictureRect = player->pictureRect();

        qreal availableWidth = m_containment->rect().width() - infoDisplayPreferredShowingRect().width();
        qreal availableHeight = m_containment->rect().height() - controllerPreferredShowingRect().height();
        qreal availableLeft = 0;
        qreal availableTop = controllerPreferredShowingRect().height();

        qreal pictureRatio = pictureRect.width() / pictureRect.height();
        qreal availableRatio = availableWidth / availableHeight;

        //Include margin

        qreal newAvailableWidth = availableWidth - ((m_containment->rect().width() - infoDisplayPreferredShowingRect().width()) * MARGINFACTOR);
        qreal newAvailableHeight = availableHeight - ((m_containment->rect().height() - controllerPreferredShowingRect().height()) * MARGINFACTOR);
        qreal newAvailableLeft = availableLeft + ((availableWidth * MARGINFACTOR) / 2);
        qreal newAvailableTop = availableTop + ((availableHeight * MARGINFACTOR) / 2);
        QRectF availableRect = QRectF(newAvailableLeft, newAvailableTop, newAvailableWidth, newAvailableHeight);

        qreal availablePictureWidthRatio = newAvailableWidth / pictureRect.width();
        qreal availablePictureHeightRatio = newAvailableHeight / pictureRect.height();

        //TODO: Handle cases where picture is smaller than available space --> resize?

        //Landscape Pictures
        if (pictureRatio <= availableRatio && pictureRect.height() <= pictureRect.width()) {
            m_player->setPos((availableRect.left() + (availableRect.width() / 2)) - ((pictureRect.width() * availablePictureHeightRatio / 2)),
                               availableRect.top());
            m_player->resize(pictureRect.width() * availablePictureHeightRatio - 7 ,availableRect.height() - 5);
        }
        if (pictureRatio >= availableRatio && pictureRect.height() <= pictureRect.width()) {
            m_player->setPos(availableRect.left(),
                             (((m_containment->rect().height() - (m_containment->rect().height() * MARGINFACTOR)/2  ) - availableRect.top())/2 + availableRect.top()) - (pictureRect.height() * availablePictureWidthRatio / 2));
            m_player->resize(availableRect.width() - 5 ,pictureRect.height() * availablePictureWidthRatio - 4.2);
        }

        //Portrait Pictures
        if (pictureRatio <= availableRatio && pictureRect.height() >= pictureRect.width()) {
            m_player->setPos((availableRect.left() + (availableRect.width() / 2)) - ((pictureRect.width() * availablePictureHeightRatio / 2)),
                               availableRect.top());
            m_player->resize(pictureRect.width() * availablePictureHeightRatio - 4.2 ,availableRect.height() - 5);
        }
        if (pictureRatio >= availableRatio && pictureRect.height() >= pictureRect.width()) {
            m_player->setPos(availableRect.left(),
                             (((m_containment->rect().height() - (m_containment->rect().height() * MARGINFACTOR)/2  ) - availableRect.top())/2 + availableRect.top()) - (pictureRect.height() * availablePictureWidthRatio / 2));
            m_player->resize(availableRect.width() ,pictureRect.height() * availablePictureWidthRatio);
        }
        //BUG:The player needs to be correctly repainted after this resize. currently the correct repainting happens only on resize of window?!
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
        m_player->setPos(0,0);
        m_player->resize(m_containment->size());
    }
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
    return QRectF(QPointF(-leftMargin, (controllerPreferredShowingRect().size().height())),
                  QSizeF(m_containment->size().width(), m_containment->size().height() -
                  controllerPreferredShowingRect().size().height()));
}

QRectF MediaLayout::controllerPreferredShowingRect() const
{
    if (!m_control) {
        return QRectF();
    }

    const int width = m_containment->size().width();
    const int height = 50;

    return QRectF(QPointF(0, 0), QSizeF(width, height));
}

QRectF MediaLayout::infoDisplayPreferredShowingRect() const
{
    if (!m_infoDisplay) {
        return QRectF();
    }
    if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
        const int width = m_containment->size().width();
        const int height = 50;
        //kWarning() << "we send out bottom"<< QRectF(QPointF(0, m_containment->size().height() - height), QSizeF(width, height));
        //BUG: at startup, the containment size is not immediately correct, causing the controller to be positioned incorrectly,
        //using the Windowstate change event makes it correct, but causes a lot of repainting
        return QRectF(QPointF(0, m_containment->size().height() - height), QSizeF(width, height));
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {

        const int leftMargin = m_containment->size().width() * 0.69;
        const int width = m_containment->size().width() * 0.3;
        const int height = m_containment->size().height() - controllerPreferredShowingRect().size().height() - (controllerPreferredShowingRect().size().height() / 6);
        //kWarning() << "we send out floating" << QRectF(QPointF(leftMargin , controllerPreferredShowingRect().height() + (controllerPreferredShowingRect().height() / 6)), QSizeF(width, height));
        return QRectF(QPointF(leftMargin , controllerPreferredShowingRect().height() + (controllerPreferredShowingRect().height() / 6)), QSizeF(width, height));
    }
}

QRectF MediaLayout::playlistPreferredShowingRect() const
{
    if (!m_playlist) {
        return QRectF();
    }

    qreal rightMargin;
    m_playlist->getContentsMargins(0, 0, &rightMargin, 0);
    return QRectF(QPointF(m_containment->size().width() - (m_containment->size().width() / 5.0) + rightMargin,
                           controllerPreferredShowingRect().size().height()),
                           QSizeF(m_containment->size().width() / 5.0, m_containment->size().height()));
}

void MediaLayout::animateHidingApplet(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(-m_browser->size().width(),
                                                                                              browserPreferredShowingRect().y()));
    } else if (applet == m_control) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(controllerPreferredShowingRect().x(),
                                                                                              m_control->rect().y() - m_control->size().height()));
        if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
            Plasma::Animator::self()->moveItem(m_infoDisplay, Plasma::Animator::SlideOutMovement, QPoint(infoDisplayPreferredShowingRect().x(),
                                                                                                  m_containment->size().height() + infoDisplayPreferredShowingRect().height()));
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            Plasma::Animator::self()->moveItem(m_infoDisplay, Plasma::Animator::SlideOutMovement, QPoint(m_containment->size().width(),
                                                                                                  controllerPreferredShowingRect().bottom()));
        }
    } else if (applet == m_playlist) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(m_containment->size().width(),
                                                                                              playlistPreferredShowingRect().y()));
    } else if (applet == m_infoDisplay) {
        if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
            Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(infoDisplayPreferredShowingRect().x(),
                                                                                                  m_containment->size().height() + infoDisplayPreferredShowingRect().height()));
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideOutMovement, QPoint(m_containment->size().width(),
                                                                                                  controllerPreferredShowingRect().bottom()));
        }
        Plasma::Animator::self()->moveItem(m_control, Plasma::Animator::SlideOutMovement, QPoint(controllerPreferredShowingRect().x(),
                                                                                              m_control->rect().y() - m_control->size().height()));
    }
}

void MediaLayout::animateShowingApplet(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, browserPreferredShowingRect().topLeft().toPoint());
    } else if (applet == m_control) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, controllerPreferredShowingRect().topLeft().toPoint());
        if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
            Plasma::Animator::self()->moveItem(m_infoDisplay, Plasma::Animator::SlideInMovement, infoDisplayPreferredShowingRect().topLeft().toPoint());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            Plasma::Animator::self()->moveItem(m_infoDisplay, Plasma::Animator::SlideInMovement, infoDisplayPreferredShowingRect().topLeft().toPoint());
        }

    } else if (applet == m_playlist) {
        Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, playlistPreferredShowingRect().topLeft().toPoint());
    } else if (applet == m_infoDisplay) {
        if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
            Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, infoDisplayPreferredShowingRect().topLeft().toPoint());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            Plasma::Animator::self()->moveItem(applet, Plasma::Animator::SlideInMovement, infoDisplayPreferredShowingRect().topLeft().toPoint());
        }
        Plasma::Animator::self()->moveItem(m_control, Plasma::Animator::SlideInMovement, controllerPreferredShowingRect().topLeft().toPoint());
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
        if (m_infoDisplay) {
            animateShowingApplet(m_infoDisplay);
            setEnableGlowHandler(m_infoDisplay, false);
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
        if (m_infoDisplay) {
            animateHidingApplet(m_infoDisplay);
            setEnableGlowHandler(m_infoDisplay, true);
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
    layoutBrowser();
}

void MediaLayout::toggleControlAutohide()
{
    //FIXME:The whole m_controlAutohide true/false settings needs to be reworked for the whole nedialyut class
    if (m_controlAutohide == true) {
        animateHidingApplet(m_control);
        animateHidingApplet(m_infoDisplay);
        m_controlAutohide = false;
        connect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        connect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    } else {
        animateShowingApplet(m_control);
        animateShowingApplet(m_infoDisplay);
        m_controlAutohide = true;
        disconnect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    }
    layoutBrowser();
    layoutPlaylist();
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

void MediaLayout::showBrowser()
{
    m_browser->setVisible(true);
    m_player->setVisible(false);
}

void MediaLayout::showPlayer()
{
    m_browser->setVisible(false);
    m_player->setVisible(true);
}

void MediaLayout::showPlaylist()
{
    setPlaylistVisible(true);
}

void MediaLayout::hidePlaylist()
{
    setPlaylistVisible(false);
}

void MediaLayout::controlAutohideOn()
{
    if (m_controlAutohide) {
        setControlAutohide(true);
    }
}

void MediaLayout::controlAutohideOff()
{
    if (!m_controlAutohide) {
        setControlAutohide(false);
    }
}

void MediaLayout::setInfoDisplayOnly(bool set)
{
    m_infoDisplayOnly = set;
    if (set) {
        m_infoDisplay->setVisible(true);
        m_control->setVisible(false);
        controlAutohideOff();
    }
    if (!set) {
        m_infoDisplay->setVisible(false);
        m_control->setVisible(true);
        controlAutohideOn();
    }
    layoutBrowser();
}

void MediaLayout::setInfoDisplayMode(const MediaCenter::InfoDisplayMode &mode)
{
    if (m_infoDisplayMode == mode) {
        return;
    }
    m_infoDisplayMode = mode;

    if (mode == MediaCenter::InfoDisplayBottom) {
        connect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    }
    if (mode == MediaCenter::InfoDisplayFloating) {
        disconnect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
    }

    doCompleteLayout();
}

MediaCenter::InfoDisplayMode MediaLayout::infoDisplayMode() const
{
    return m_infoDisplayMode;
}

