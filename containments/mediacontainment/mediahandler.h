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
#ifndef MEDIAHANDLER_H
#define MEDIAHANDLER_H

#include <QGraphicsWidget>
#include <QHash>

namespace Plasma {
    class FrameSvg;
    class Svg;
    class Applet;
}

class QGraphicsSceneResizeEvent;
class QGraphicsSceneHoverEvent;

class MediaHandler : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum HandlerPosition {
        Right,
        Left,
        Top,
        Bottom
    };
    MediaHandler(Plasma::Applet *applet, HandlerPosition position = Right);
    ~MediaHandler();

    void setHandlerPosition(HandlerPosition);
    HandlerPosition handlerPosition();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool eventFilter(QObject *o, QEvent *e);

    /**
     * Call this method whenever is needed to stop
     * glowing animation due to hover events over the
     * handler. This might be useful while moving the applet
     * since can happen that the applet passes under the mouse
     * cursor causing an hoverEvent to be processed.
     */
    void setStopHoverEvents(bool set);
    bool stopHoverEvents();

    /**
     * Call this method in order to retrieve the MediaHandler
     * associated with the given applet.
     *
     * @param: the applet expected to be associated with an
     * handler.
     * @return: the associated handler or 0 if none is found.
     */
    static MediaHandler* handlerFromApplet(Plasma::Applet*);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

protected slots:
    virtual void animateShowHide(qreal);

signals:
    /**
     * emitted whenever the handler requests the applet
     * to be shown.
     */
    void appletShowRequest(Plasma::Applet *);

    /**
     * emitted whenever the handler requests the applet
     * to be hidden.
     */
    void appletHideRequest(Plasma::Applet *);

private:
    HandlerPosition m_handlerPosition;
    Plasma::Applet *m_applet;
    Plasma::FrameSvg *m_handlerSvg;
    qreal m_showFactor;
    bool m_appearing;
    bool m_appletVisible;
    bool m_stopHovers;
    int m_animationId;

    static QHash<Plasma::Applet*, MediaHandler*> m_handlers;

private:
    void enableBordersByPosition();
    void hidingAnimation();
    void showingAnimation();
};

#endif // MEDIAHANDLER_H
