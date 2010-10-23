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
#include <QImageReader>

#include <KDebug>

#include <Plasma/Containment>
#include <Plasma/Animator>
#include <Plasma/Theme>

using namespace MediaCenter;

static const qreal MARGINFACTOR = 0.2; //used when laying out the player in floating mode
static const qreal PLAYLISTSIZEFACTOR = 3.5; //used to define the ratio playlist width to total width

MediaLayout::MediaLayout(Plasma::Containment *parent) : QObject(parent),
m_containment(parent),
m_browser(0),
m_control(0),
m_playlist(0),
m_player(0),
m_infoDisplay(0),
m_homeApplet(0),
m_showAll(false),
m_playlistVisible(false),
m_controlAutohide(false),
m_controlVisible(true),
m_playlistHandler(0),
m_controlHandler(0),
m_infoDisplayHandler(0),
m_infoDisplayOnly(true),
m_infoDisplayMode(MediaCenter::InfoDisplayBottom),
m_playerNewRect(QRectF())

{
    m_containment->installEventFilter(this);

    m_resizeBrowserAnimation = Plasma::Animator::create(Plasma::Animator::GeometryAnimation);
    m_resizePlaylistAnimation = Plasma::Animator::create(Plasma::Animator::GeometryAnimation);
    m_resizePlayerAnimation = Plasma::Animator::create(Plasma::Animator::GeometryAnimation);
    m_resizeInfoDisplayAnimation = Plasma::Animator::create(Plasma::Animator::GeometryAnimation);

    m_slidePlaylistAnimation = Plasma::Animator::create(Plasma::Animator::SlideAnimation);
    m_slideControlAnimation = Plasma::Animator::create(Plasma::Animator::SlideAnimation);
    m_slideInfoDisplayAnimation = Plasma::Animator::create(Plasma::Animator::SlideAnimation);

    m_fadeOutPlayerAnimation = Plasma::Animator::create(Plasma::Animator::FadeAnimation);
    m_fadeInPlayerAnimation = Plasma::Animator::create(Plasma::Animator::FadeAnimation);

    //Some applets (i.e. playlist) change position depending on the applied theme.
    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(doCompleteLayout()));
}

MediaLayout::~MediaLayout()
{
}

void MediaLayout::setBrowser(Plasma::Applet *browser)
{
    m_browser = browser;
    m_needLayouting << m_browser;
    m_browser->setZValue(10);

    m_browserBackgroundHints = m_browser->backgroundHints();
}

void MediaLayout::setPlaybackControl(Plasma::Applet *control)
{
    m_control = control;
    m_needLayouting << m_control;
    m_control->setZValue(1000);
    m_controlHandler = new MediaHandler(m_control, MediaHandler::Bottom);
}

void MediaLayout::setInfoDisplay(Plasma::Applet *infoDisplay)
{
    m_infoDisplay = infoDisplay;
    m_needLayouting << m_infoDisplay;
    m_infoDisplay->setZValue(2000);
    m_infoDisplayHandler = new MediaHandler(m_infoDisplay, MediaHandler::Top);
}

void MediaLayout::setPlaylist(Plasma::Applet *playlist)
{
    m_playlist = playlist;
    m_needLayouting << m_playlist;
    m_playlist->setZValue(999);
    m_playlistHandler = new MediaHandler(m_playlist, MediaHandler::Left);
}

void MediaLayout::setPlayer(Plasma::Applet *player)
{
    m_player = player;
    m_player->setZValue(-50);
    m_needLayouting << m_player;
}

void MediaLayout::setHomeApplet(Plasma::Applet *home)
{
    m_homeApplet = home;
    m_homeApplet->setZValue(5000);
    m_needLayouting << m_homeApplet;
}

void MediaLayout::invalidate()
{
    if (m_needLayouting.isEmpty()) {
        //This allows calling doCompleteLayout from outside of medialayout
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
        } else if (applet == m_homeApplet) {
            layoutHomeApplet();
        }
     }
     m_needLayouting.clear();
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
    if (m_player  && m_player->isVisible()) {
        layoutPlayer();
    }
    if (m_infoDisplay) {
        layoutInfoDisplay();
    }
    if (m_homeApplet) {
        layoutHomeApplet();
    }
}

void MediaLayout::layoutBrowser()
{
    QPointF point;
    QSizeF size;

    if (m_playlistVisible) {
        if (m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top());
            size = QSizeF(m_containment->size().width() - playlistPreferredShowingRect().size().width(), m_containment->size().height());
        }
        if (!m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            size = QSizeF(m_containment->size().width() - playlistPreferredShowingRect().size().width(),
                        m_containment->size().height() - controllerPreferredShowingRect().size().height() - infoDisplayPreferredShowingRect().size().height());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            size = QSizeF(infoDisplayPreferredShowingRect().left(), m_containment->size().height() -
                              controllerPreferredShowingRect().size().height());
        }
    }
    if (!m_playlistVisible) {
        if (m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top());
            size = QSizeF(m_containment->size().width(), m_containment->size().height());
        }
        if (!m_controlAutohide) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            size = QSizeF(m_containment->size().width(), m_containment->size().height() -
                            controllerPreferredShowingRect().size().height() - infoDisplayPreferredShowingRect().size().height());
        }
        if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
            m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
            point = QPointF(m_containment->rect().left(), m_containment->rect().top() +
                              controllerPreferredShowingRect().size().height());
            size = QSizeF(infoDisplayPreferredShowingRect().left(), m_containment->size().height() -
                              controllerPreferredShowingRect().size().height());
        }
    }
    if (m_infoDisplayOnly) {
        m_browser->setBackgroundHints(Plasma::Applet::NoBackground);
        point = QPointF(m_containment->rect().left(), m_containment->rect().top());
        size = QSizeF(m_containment->size().width(), m_containment->size().height() - infoDisplayPreferredShowingRect().size().height());
    }

    animateAppletResize(m_browser, QRectF(point,size));
}

void MediaLayout::layoutControl()
{
    if (m_controlAutohide) {
        m_control->setPos(QPointF(0, 0 - controllerPreferredShowingRect().height()));
        m_control->resize(controllerPreferredShowingRect().size());
    }
    if (!m_controlAutohide) {
        m_control->setPos(QPointF(0, 0));
        m_control->resize(controllerPreferredShowingRect().size());
    }
}

void MediaLayout::layoutInfoDisplay()
{
    if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
        if (m_controlAutohide) {
            m_infoDisplay->setPos(QPointF(0, m_containment->rect().bottom()));
            m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
        }
        if (!m_controlAutohide) {
            m_infoDisplay->setPos(QPointF(0, m_containment->rect().bottom() -
                                infoDisplayPreferredShowingRect().height()));
            m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
        }
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {

        m_infoDisplay->setPos(QPointF(infoDisplayPreferredShowingRect().left(), infoDisplayPreferredShowingRect().top()));
        m_infoDisplay->resize(infoDisplayPreferredShowingRect().size());
    }
}

void MediaLayout::layoutPlaylist()
{
    qreal left = 0;
    qreal top = 0;
    qreal right = 0;
    qreal bottom = 0;
    Plasma::FrameSvg *svg = new Plasma::FrameSvg(this);

    //The following part determines the margin size depending on the background of the widget
    QString imagePath;
    if (m_playlist->backgroundHints() == Plasma::Applet::StandardBackground) {
        imagePath = Plasma::Theme::defaultTheme()->imagePath("widgets/background");
        svg->setImagePath(imagePath);
        svg->getMargins(left,top,right,bottom);
    } else if (m_playlist->backgroundHints() == Plasma::Applet::TranslucentBackground) {
        imagePath = Plasma::Theme::defaultTheme()->imagePath("widgets/translucentbackground");
        svg->setImagePath(imagePath);
        svg->getMargins(left,top,right,bottom);
    } else if (m_playlist->backgroundHints() == Plasma::Applet::NoBackground) {
        //FIXME: Currently this supposes that the playlist is painting it's own background and
        //uses the standard widgets/background. In case we get custom playlists, this
        //needs to be changed
        imagePath = Plasma::Theme::defaultTheme()->imagePath("widgets/background");
        svg->setImagePath(imagePath);
        svg->getMargins(left,top,right,bottom);
    }

    QPointF point;
    QSizeF size;

    if (m_playlistVisible) {
        if (!m_controlVisible && m_controlAutohide) {
            point = QPointF(playlistPreferredShowingRect().left(),0);
            size = QSizeF(playlistPreferredShowingRect().size());
        }
        if (m_controlVisible && !m_controlAutohide) {
            point = QPointF(playlistPreferredShowingRect().left(), controllerPreferredShowingRect().bottom() - top/2);
            size = QSizeF(playlistPreferredShowingRect().size().width(), playlistPreferredShowingRect().size().height() -
                         infoDisplayPreferredShowingRect().size().height() - controllerPreferredShowingRect().size().height() + bottom/2 + top/2);
        }
        if (!m_controlVisible && m_controlAutohide) {
            point = QPointF(playlistPreferredShowingRect().left(),0);
            size = QSizeF(playlistPreferredShowingRect().size());
        }
    }
    if (!m_playlistVisible) {
        if (!m_controlVisible && m_controlAutohide) {
            point = QPointF(playlistPreferredShowingRect().right(), 0);
            m_playlist->setPos(m_containment->rect().right(), 0); //Without this manual positioning, the playlist is at 50 px from top when toggling in video playing state
            size = QSizeF(playlistPreferredShowingRect().size());
        }
        if (m_controlVisible && !m_controlAutohide) {
            if (m_infoDisplayOnly) {
                point = QPointF(playlistPreferredShowingRect().right(), 0);
                size = QSizeF(playlistPreferredShowingRect().size());
            }
            if (!m_infoDisplayOnly) {
                point = QPointF(playlistPreferredShowingRect().right(), controllerPreferredShowingRect().bottom() - top/2);
                size = QSizeF(playlistPreferredShowingRect().size().width(), playlistPreferredShowingRect().size().height() -
                             infoDisplayPreferredShowingRect().size().height() - controllerPreferredShowingRect().size().height()  + bottom/2 + top/2);
            }
        }
        if (m_controlVisible && m_controlAutohide) {
            point = QPointF(playlistPreferredShowingRect().right(), 0);
            m_playlist->setPos(m_containment->rect().right(), 0); //Without this manual positioning, the playlist is at 50 px from top when toggling in video playing state
            size = QSizeF(playlistPreferredShowingRect().size());
        }
    }
    animateAppletResize(m_playlist, QRectF(point,size));
}

void MediaLayout::layoutPlayer()
{
    kDebug() << "changing player layout to" << m_infoDisplayMode;
    m_player->setMinimumSize(50,50);

    QPointF point;
    QSizeF size;

    if (m_infoDisplayMode == MediaCenter::InfoDisplayBottom) {
        point = QPointF(0,0);
        size = QSizeF(m_containment->size());
        m_player->setGeometry(QRectF(point,size));
        disconnect(m_player, SIGNAL(newMedia(MediaCenter::Media)), this, SLOT(fadeInPlayer()));
        return;
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(m_player);

        if (!player) {
            return;
        }
        connect(player, SIGNAL(newMedia(MediaCenter::Media)), this, SLOT(fadeInPlayer()));
        connect(m_fadeOutPlayerAnimation, SIGNAL(finished()), this, SLOT(fadeInPlayer()));

        const QSize pictureSize =  QImageReader(player->currentMedia(MediaCenter::PictureMode).second).size();
        QRectF pictureRect = QRectF(0,0, pictureSize.width(), pictureSize.height());

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
            point = QPointF((availableRect.left() + (availableRect.width() / 2)) - ((pictureRect.width() * availablePictureHeightRatio / 2)),
                               availableRect.top());
            size = QSizeF(pictureRect.width() * availablePictureHeightRatio - 7 ,availableRect.height() - 5);
        }
        if (pictureRatio >= availableRatio && pictureRect.height() <= pictureRect.width()) {
            point = QPointF(availableRect.left(),
                             (((m_containment->rect().height() - (m_containment->rect().height() * MARGINFACTOR)/2  ) - availableRect.top())/2 + availableRect.top()) - (pictureRect.height() * availablePictureWidthRatio / 2));
            size = QSizeF(availableRect.width() - 5 ,pictureRect.height() * availablePictureWidthRatio - 4.2);
        }

        //Portrait Pictures
        if (pictureRatio <= availableRatio && pictureRect.height() >= pictureRect.width()) {
            point = QPointF((availableRect.left() + (availableRect.width() / 2)) - ((pictureRect.width() * availablePictureHeightRatio / 2)),
                               availableRect.top());
            size = QSizeF(pictureRect.width() * availablePictureHeightRatio - 4.2 ,availableRect.height() - 5);
        }
        if (pictureRatio >= availableRatio && pictureRect.height() >= pictureRect.width()) {
            point = QPointF(availableRect.left(),
                             (((m_containment->rect().height() - (m_containment->rect().height() * MARGINFACTOR)/2  ) - availableRect.top())/2 + availableRect.top()) - (pictureRect.height() * availablePictureWidthRatio / 2));
            size = QSizeF(availableRect.width() ,pictureRect.height() * availablePictureWidthRatio);
        }
    }

    m_playerNewRect = QRectF(point,size);

    m_fadeOutPlayerAnimation->setProperty("startOpacity", 1); //FIXME:Fade in works, but fade out doesn't. Why ?
    m_fadeOutPlayerAnimation->setProperty("targetOpacity", 0);
    m_fadeOutPlayerAnimation->setTargetWidget(m_player);
    m_fadeOutPlayerAnimation->start();
}

void MediaLayout::layoutHomeApplet()
{
    m_homeApplet->setGeometry(homeAppletPreferredShowingRect());
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
        return QRectF(QPointF(0, m_containment->rect().bottom() - height), QSizeF(width, height));
    }

    if (m_infoDisplayMode == MediaCenter::InfoDisplayFloating) {
        const int leftMargin = m_containment->size().width() * 0.69;
        const int width = m_containment->size().width() * 0.3;
        const int height = m_containment->size().height() - controllerPreferredShowingRect().size().height() - (controllerPreferredShowingRect().size().height() / 6);
        return QRectF(QPointF(leftMargin , controllerPreferredShowingRect().height() + (controllerPreferredShowingRect().height() / 6)), QSizeF(width, height));
    }

    return QRectF();
}

QRectF MediaLayout::playlistPreferredShowingRect() const
{
    if (!m_playlist) {
        return QRectF();
    }

    return QRectF(QPointF(m_containment->size().width() - (m_containment->size().width() / PLAYLISTSIZEFACTOR),
                           controllerPreferredShowingRect().size().height()),
                           QSizeF(m_containment->size().width() / PLAYLISTSIZEFACTOR, m_containment->size().height()));
}

QRectF MediaLayout::homeAppletPreferredShowingRect() const
{
    if (!m_homeApplet) {
        return QRectF();
    }

    return m_containment->rect();
}

void MediaLayout::animateHidingApplet(Plasma::Applet *applet)
{
    if ((applet == m_control && m_slideControlAnimation->state() == Plasma::Animation::Running) ||
        (applet == m_infoDisplay && m_slideInfoDisplayAnimation->state() == Plasma::Animation::Running) ||
        (applet == m_playlist && m_slidePlaylistAnimation->state() == Plasma::Animation::Running)) {
        return;
    }

    m_slidePlaylistAnimation->setProperty("movementDirection", Plasma::Animation::MoveRight);
    m_slideControlAnimation->setProperty("movementDirection", Plasma::Animation::MoveUp);
    m_slidePlaylistAnimation->setProperty("distance", m_playlist->rect().right());
    m_slidePlaylistAnimation->setTargetWidget(m_playlist);
    m_slideControlAnimation->setProperty("distance", m_control->rect().bottom());
    m_slideControlAnimation->setTargetWidget(m_control);
    if (infoDisplayMode() == MediaCenter::InfoDisplayBottom) {
        m_slideInfoDisplayAnimation->setProperty("movementDirection", Plasma::Animation::MoveDown);
        m_slideInfoDisplayAnimation->setProperty("distance", m_infoDisplay->rect().bottom());
        m_slideInfoDisplayAnimation->setTargetWidget(m_infoDisplay);
    }
    if (infoDisplayMode() == MediaCenter::InfoDisplayFloating) {
        m_slideInfoDisplayAnimation->setProperty("movementDirection", Plasma::Animation::MoveRight);
        m_slideInfoDisplayAnimation->setProperty("distance", m_infoDisplay->rect().right());
        m_slideInfoDisplayAnimation->setTargetWidget(m_infoDisplay);
    }

    if (applet == m_browser) {
    } else if (applet == m_control || applet == m_infoDisplay) {
        if (m_controlVisible) {
            m_slideControlAnimation->start();
            m_slideInfoDisplayAnimation->start();
            m_controlVisible = false;
        }
    } else if (applet == m_playlist) {
        if (m_playlistVisible) {
            m_slidePlaylistAnimation->start();
            m_playlistVisible = false;
        }
    }
}

void MediaLayout::animateShowingApplet(Plasma::Applet *applet)
{
    if ((applet == m_control && m_slideControlAnimation->state() == Plasma::Animation::Running) ||
        (applet == m_infoDisplay && m_slideInfoDisplayAnimation->state() == Plasma::Animation::Running) ||
        (applet == m_playlist && m_slidePlaylistAnimation->state() == Plasma::Animation::Running)) {
        return;
    }

    m_slidePlaylistAnimation->setProperty("movementDirection", Plasma::Animation::MoveLeft);
    m_slideControlAnimation->setProperty("movementDirection", Plasma::Animation::MoveDown);
    m_slidePlaylistAnimation->setProperty("distance", m_playlist->rect().right());
    m_slidePlaylistAnimation->setTargetWidget(m_playlist);
    m_slideControlAnimation->setProperty("distance", m_control->rect().bottom());
    m_slideControlAnimation->setTargetWidget(m_control);
    if (infoDisplayMode() == MediaCenter::InfoDisplayBottom) {
        m_slideInfoDisplayAnimation->setProperty("movementDirection", Plasma::Animation::MoveUp);
        m_slideInfoDisplayAnimation->setProperty("distance", m_infoDisplay->rect().bottom());
        m_slideInfoDisplayAnimation->setTargetWidget(m_infoDisplay);
    }
    if (infoDisplayMode() == MediaCenter::InfoDisplayFloating) {
        m_slideInfoDisplayAnimation->setProperty("movementDirection", Plasma::Animation::MoveLeft);
        m_slideInfoDisplayAnimation->setProperty("distance", m_infoDisplay->rect().right());
        m_slideInfoDisplayAnimation->setTargetWidget(m_infoDisplay);
    }

    if (applet == m_browser) {
    } else if (applet == m_control || applet == m_infoDisplay) {
        if (!m_controlVisible) {
            m_slideControlAnimation->start();
            m_slideInfoDisplayAnimation->start(); 
            m_controlVisible = true;
        }
    } else if (applet == m_playlist) {
        if (!m_playlistVisible) {
            layoutPlaylist(); //necessary for correct placement at startup
            m_slidePlaylistAnimation->start();
            m_playlistVisible = true;
        }
    }
}

void MediaCenter::MediaLayout::animateAppletResize(Plasma::Applet *applet, QRectF target)
{
    if (applet == m_browser) {
        if (m_resizeBrowserAnimation->state() == Plasma::Animation::Running) {
            m_resizeBrowserAnimation->stop();
        }
        m_resizeBrowserAnimation->setTargetWidget(m_browser);
        m_resizeBrowserAnimation->setProperty("startGeometry", m_browser->geometry());
        m_resizeBrowserAnimation->setProperty("targetGeometry", target);
        m_resizeBrowserAnimation->start();
    }
    if (applet == m_playlist) {
        if (m_resizePlaylistAnimation->state() == Plasma::Animation::Running) {
            m_resizePlaylistAnimation->stop();
        }
        m_resizePlaylistAnimation->setTargetWidget(m_playlist);
        m_resizePlaylistAnimation->setProperty("startGeometry", m_playlist->geometry());
        m_resizePlaylistAnimation->setProperty("targetGeometry", target);
        m_resizePlaylistAnimation->start();
    }
    if (applet == m_player) {
        if (m_resizePlayerAnimation->state() == Plasma::Animation::Running) {
            m_resizePlayerAnimation->stop();
        }
        m_resizePlayerAnimation->setTargetWidget(m_player);
        m_resizePlayerAnimation->setProperty("startGeometry", m_player->geometry());
        m_resizePlayerAnimation->setProperty("targetGeometry", target);
        m_resizePlayerAnimation->start();
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
        connect (m_playlistHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_playlistHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        layoutBrowser();
        return;
    } else {
        animateShowingApplet(m_playlist);
        disconnect (m_playlistHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_playlistHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        layoutBrowser();
        return;
    }
}

void MediaLayout::toggleControlAutohide()
{
    if (m_controlAutohide == false) {
        animateHidingApplet(m_control);
        m_controlAutohide = true;
        connect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        connect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        connect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        layoutBrowser();
        layoutPlaylist();
        return;
    }
    if (m_controlAutohide == true) {
        animateShowingApplet(m_control);
        m_controlAutohide = false;
        disconnect (m_controlHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_controlHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        layoutBrowser();
        layoutPlaylist();
        return;
    }
}

void MediaLayout::setControlAutohide(bool set) {
    if (m_controlAutohide == set) {
        return;
    }
    //Since we are going to toggle now, we need to set autoHide to the opposite
    //of the wanted setting.
    m_controlAutohide = !set;
    toggleControlAutohide();
}

bool MediaLayout::playlistVisible() {
    return m_playlistVisible;
}

void MediaLayout::setPlaylistVisible (bool set) {
    if (m_playlistVisible == set) {
        return;
    }
    //Since we are going to toggle now, we need to set autoHide to the opposite
    //of the wanted setting.
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
    setControlAutohide(true);
}

void MediaLayout::controlAutohideOff()
{
    setControlAutohide(false);
}

void MediaLayout::setInfoDisplayOnly(bool set)
{
    m_infoDisplayOnly = set;
    if (set) {
        m_infoDisplay->setVisible(true);
        m_control->setVisible(false);
        controlAutohideOff();
        disconnect (m_infoDisplayHandler, SIGNAL(appletHideRequest(Plasma::Applet*)), this, SLOT(animateHidingApplet(Plasma::Applet*)));
        disconnect (m_infoDisplayHandler, SIGNAL(appletShowRequest(Plasma::Applet*)), this, SLOT(animateShowingApplet(Plasma::Applet*)));
        //Just to make sure they are never connected in the homestate
    }
    if (!set) {
        m_infoDisplay->setVisible(true);
        m_control->setVisible(true);
    }
    layoutBrowser();
    layoutPlaylist(); //Necessary for correct playlist placement on startup
}

void MediaLayout::setInfoDisplayMode(const MediaCenter::InfoDisplayMode &mode)
{
    if (m_infoDisplayMode == mode) {
        return;
    }
    m_infoDisplayMode = mode;

    layoutInfoDisplay();
    layoutPlayer();
    layoutBrowser();
}

MediaCenter::InfoDisplayMode MediaLayout::infoDisplayMode() const
{
    return m_infoDisplayMode;
}

void MediaLayout::fadeInPlayer()
{
    m_player->setGeometry(m_playerNewRect);
    disconnect(m_fadeOutPlayerAnimation, SIGNAL(finished()), this, SLOT(fadeInPlayer()));

    m_fadeInPlayerAnimation->setProperty("startOpacity", 0);
    m_fadeInPlayerAnimation->setProperty("targetOpacity", 1);
    m_fadeInPlayerAnimation->setTargetWidget(m_player);
    m_fadeInPlayerAnimation->start();
}
