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
#include "mediahandler.h"

#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneHoverEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <KDebug>

#include <Plasma/Applet>
#include <Plasma/FrameSvg>
#include <Plasma/Svg>
#include <Plasma/Animator>

static const int WIDTH = 60;

MediaHandler::MediaHandler(Plasma::Applet *applet, HandlerPosition position) : QGraphicsWidget(applet),
m_handlerPosition(position),
m_applet(applet),
m_handlerSvg(new Plasma::FrameSvg(this)),
m_showFactor(0),
m_appearing(false),
m_animationId(0)
{
    m_applet->installEventFilter(this);

    setZValue(m_applet->zValue() - 1);

    m_handlerSvg->setCacheAllRenderedFrames(true);
    enableBordersByPosition();
    m_handlerSvg->setImagePath("widgets/glowbar");

    setContentsMargins(0, 0, 0, 0);

    setAcceptsHoverEvents(true);
}

MediaHandler::~MediaHandler()
{}

void MediaHandler::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    if (m_handlerPosition == Left || m_handlerPosition == Right) {
        m_handlerSvg->resizeFrame(QSizeF(12, size().height()));
    } else {
        m_handlerSvg->resizeFrame(QSizeF(size().width(), 12));
    }
}

void MediaHandler::setHandlerPosition(HandlerPosition position)
{
    m_handlerPosition = position;
    enableBordersByPosition();
}

void MediaHandler::enableBordersByPosition()
{
    switch (m_handlerPosition) {
        case Right :
        m_handlerSvg->setEnabledBorders(Plasma::FrameSvg::TopBorder | Plasma::FrameSvg::RightBorder | Plasma::FrameSvg::BottomBorder);
        break;
        case Left :
        m_handlerSvg->setEnabledBorders(Plasma::FrameSvg::TopBorder | Plasma::FrameSvg::LeftBorder | Plasma::FrameSvg::BottomBorder);
        break;
        case Top :
        m_handlerSvg->setEnabledBorders(Plasma::FrameSvg::TopBorder | Plasma::FrameSvg::LeftBorder | Plasma::FrameSvg::RightBorder);
        break;
        case Bottom :
        m_handlerSvg->setEnabledBorders(Plasma::FrameSvg::BottomBorder | Plasma::FrameSvg::LeftBorder | Plasma::FrameSvg::RightBorder);
        break;
        default :
                m_handlerSvg->setEnabledBorders(Plasma::FrameSvg::TopBorder | Plasma::FrameSvg::RightBorder | Plasma::FrameSvg::BottomBorder);
    }
}

void MediaHandler::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->setOpacity(m_showFactor);
    if (m_handlerPosition == Left) {
        m_handlerSvg->paintFrame(painter, QPointF(option->rect.right() - m_handlerSvg->frameSize().width(), option->rect.top()));
    } else if (m_handlerPosition == Top) {
        m_handlerSvg->paintFrame(painter, QPointF(0, option->rect.bottom() - m_handlerSvg->frameSize().height()));
    } else {
        m_handlerSvg->paintFrame(painter, option->rect.topLeft());
    }
}

bool MediaHandler::eventFilter(QObject *o, QEvent *e)
{
    if (o != m_applet) {
        return false;
    }

    if (e->type() == QEvent::GraphicsSceneMove) {
        switch (m_handlerPosition) {
        case Right :
            setPos(m_applet->rect().right(), (m_applet->rect().height() - size().height()) / 2);
            break;
        case Top :
            setPos((m_applet->rect().width() - size().width()) / 2, m_applet->rect().top());
            break;
        case Left :
            setPos(m_applet->rect().left() - size().width(), (m_applet->rect().height() - size().height()) / 2);
            break;
        case Bottom :
            setPos((m_applet->rect().width() - size().width()) / 2, m_applet->rect().bottom());
            break;
        }
    } else if (e->type() == QEvent::GraphicsSceneResize) {
        if (m_handlerPosition == Left || m_handlerPosition == Right) {
            resize(WIDTH, m_applet->size().height());
        } else {
            resize(m_applet->size().width(), WIDTH);
        }
    } else if (e->type() == QEvent::GraphicsSceneHoverLeave) {
        emit appletHideRequest(m_applet);
        m_showFactor = 1;
    }

    return false;
}

void MediaHandler::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)

    Plasma::Animator::self()->stopCustomAnimation(m_animationId);
    m_appearing = true;
    m_showFactor = 0;
    m_animationId = Plasma::Animator::self()->customAnimation(100, 250, Plasma::Animator::EaseInCurve, this, "animateShowHide");
}

void MediaHandler::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)

    Plasma::Animator::self()->stopCustomAnimation(m_animationId);
    m_appearing = false;
    m_showFactor = 1;
    m_animationId = Plasma::Animator::self()->customAnimation(100, 250, Plasma::Animator::EaseInCurve, this, "animateShowHide");
}

void MediaHandler::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (event->pos().x() <= 10 && event->pos().x() > 0 && m_handlerPosition == Right) {
        emit appletShowRequest(m_applet);
        m_showFactor = 0;
        update();
    } else if (event->pos().y() <= 10 && event->pos().y() > 0 && m_handlerPosition == Bottom) {
        emit appletShowRequest(m_applet);
        m_showFactor = 0;
        update();
    } else if (event->pos().x() < size().width() && event->pos().x() > size().width() - 10 && m_handlerPosition == Left) {
        emit appletShowRequest(m_applet);
        m_showFactor = 0;
        update();
    } else if (event->pos().y() < size().height() && event->pos().y() > size().height() - 10 && m_handlerPosition == Top) {
        emit appletShowRequest(m_applet);
        m_showFactor = 0;
        update();
    }

}

void MediaHandler::animateShowHide(qreal value)
{
    if (!m_appearing) {
        m_showFactor = 1 - value;
        update();
        return;
    }

    m_showFactor = value;
    update();
}
