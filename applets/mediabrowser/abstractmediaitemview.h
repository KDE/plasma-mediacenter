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
#ifndef ABSTRACTMEDIAITEMVIEW_H
#define ABSTRACTMEDIAITEMVIEW_H

#include <QGraphicsWidget>

class QAbstractItemModel;
class KFileItemDelegate;

namespace Plasma {
    class ScrollBar;
}

class QScrollBar;

class AbstractMediaItemView : public QGraphicsWidget
{
    Q_OBJECT
public:
    AbstractMediaItemView(QGraphicsItem *parent = 0);
    virtual ~AbstractMediaItemView();

    void setIconSize(int size);
    int iconSize() const;

    virtual void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model();

    void setItemDelegate(KFileItemDelegate *delegate);
    KFileItemDelegate *itemDelegate();

    QScrollBar *verticalScrollBar();

    QRect contentsArea() const;

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

protected:
    int m_iconSize;
    QAbstractItemModel *m_model;
    KFileItemDelegate *m_delegate;
    Plasma::ScrollBar *m_scrollBar;
};

#endif
