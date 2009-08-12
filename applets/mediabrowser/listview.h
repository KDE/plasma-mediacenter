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
#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "abstractmediaitemview.h"
#include <QList>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class ListView : public AbstractMediaItemView
{
    Q_OBJECT
public:
    ListView(QGraphicsItem *parent = 0);
    ~ListView();

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void setupOptions();
    void layoutItems();

    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

public slots:
    void generateItems();
    void updateScrollBar();


};

#endif
