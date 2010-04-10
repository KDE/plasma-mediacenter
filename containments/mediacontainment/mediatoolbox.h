/*
 *   Copyright 2009 Marco Martin <notmart@gmail.com>
 *   Copyright 2009 Alessandro diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef MEDIATOOLBOX_H
#define MEDIATOOLBOX_H

#include <QGraphicsWidget>

#include <KIcon>

#include <Plasma/Plasma>
#include <plasma/abstracttoolbox.h>



namespace Plasma
{
    class Containment;
    class IconWidget;
    class Svg;
}


class MediaToolBox : public Plasma::AbstractToolBox
{
    Q_OBJECT
    Q_PROPERTY(bool showing READ isShowing WRITE setShowing )
public:
    MediaToolBox(Plasma::Containment *parent = 0);
    ~MediaToolBox();

    bool isShowing() const;
    void setShowing(const bool show);

    /**
     * create a toolbox tool from the given action
     * @p action the action to associate the tool with
     */
    void addTool(QAction *action);
    /**
     * remove the tool associated with this action
     */
    void removeTool(QAction *action);

    void setLocation(Plasma::Location location);
    Plasma::Location location() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

private Q_SLOTS:
    void containmentGeometryChanged();
    void animateHighlight(qreal progress);

private:
    QHash<QAction *, Plasma::IconWidget *> m_actionButtons;
    Plasma::Containment *m_containment;
    Plasma::Svg *m_background;
    KIcon m_icon;
    QSize m_iconSize;
    int m_animHighlightId;
    int m_animSlideId;
    qreal m_animHighlightFrame;
    bool m_hovering;
    bool m_showing;
    Plasma::Location m_location;
};

#endif
