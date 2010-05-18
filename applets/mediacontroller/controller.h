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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mediacenter/playbackcontrol.h>

#include <QGraphicsLinearLayout>

class MediaController : public MediaCenter::PlaybackControl
{
    Q_OBJECT
public:
     MediaController(QObject *parent, const QVariantList &args);
    virtual ~MediaController();

    void constraintsEvent(Plasma::Constraints constraints);
    void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect);
    void init();

    virtual void addToLayout(QGraphicsWidget *widget, const MediaCenter::LayoutZone &zone);
    void addLayouts();
    void resetLayouts();

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
    void slotThemeChanged();

private:
    Plasma::FrameSvg *m_svg;

    QGraphicsLinearLayout *m_layoutVertical;
    QGraphicsLinearLayout *m_layoutHorizontalTop;
    QGraphicsLinearLayout *m_layoutHorizontalBottom;
    QGraphicsLinearLayout *m_layoutTopLeft;
    QGraphicsLinearLayout *m_layoutTopMiddle;
    QGraphicsLinearLayout *m_layoutTopRight;
    QGraphicsLinearLayout *m_layoutBottomLeft;

    int iconSizeFromCurrentSize() const;

    void setMarginsFromTheme();
};

#endif
