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
#include <QStyleOptionGraphicsItem>

#include <Plasma/Applet>
#include <Plasma/FrameSvg>
#include <Plasma/Svg>

static const int ARROW_SIZE = 16;

MediaHandler::MediaHandler(Plasma::Applet *applet, HandlerPosition position) : QGraphicsWidget(applet),
m_handlerPosition(position),
m_applet(applet),
m_handlerSvg(new Plasma::FrameSvg(this)),
m_arrowSvg(new Plasma::Svg(this))
{
    m_applet->installEventFilter(this);

    setZValue(m_applet->zValue() - 1);

    m_handlerSvg->setCacheAllRenderedFrames(true);
    enableBordersByPosition();
    m_handlerSvg->setImagePath("widgets/background");

    m_arrowSvg->setContainsMultipleImages(true);
    m_arrowSvg->setImagePath("widgets/arrows");
    m_arrowSvg->resize(ARROW_SIZE, ARROW_SIZE);
}

MediaHandler::~MediaHandler()
{}

void MediaHandler::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    m_handlerSvg->resizeFrame(QSizeF(event->newSize().width(), event->newSize().width()));
}

void MediaHandler::setHandlerPosition(HandlerPosition position)
{
    m_handlerPosition = position;
    enableBordersByPosition();
}

QString MediaHandler::arrowPrefixByPosition()
{
    switch (m_handlerPosition) {
        case Right :
        return "right-arrow";
        case Left :
        return "left-arrow";
        case Top :
        return "up-arrow";
        case Bottom:
        return "down-arrow";
        default :
                return "right-arrow";
    }
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
    Q_UNUSED(widget)

    m_handlerSvg->paintFrame(painter, option->rect.topLeft());
    // TODO: draw the arrow
}

bool MediaHandler::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_applet && e->type() == QEvent::GraphicsSceneMove) {
        // TODO: move the handler following applet's position
    }

    return false;
}
