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
#include <QStyleOptionViewItemV4>

class QAbstractItemModel;
class QAbstractItemDelegate;

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

    void setItemDelegate(QAbstractItemDelegate *delegate);
    QAbstractItemDelegate *itemDelegate();

    void setRootIndex(const QModelIndex &);
    QModelIndex rootIndex() const;

    QScrollBar *verticalScrollBar();

    void hideVerticalScrollBar();
    bool isVerticalScrollBarHidden();

    QRect contentsArea() const;

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    /**
     * Reimplement this method in order to fill in m_option. Remember
     * to always call the base implementation since it already initializes
     * m_option with the basic stuff. All you need to do is specify things
     * specific for the kind of view you are going to implement (icon view or list view).
     */
    virtual void setupOptions();

protected slots:
    /**
     * Use this slot in order to update scrollBar range.
     */
    void updateScrollBar();

signals:
    void scrollOffsetChanged(int);

protected:
    int m_iconSize;
    QAbstractItemModel *m_model;
    QAbstractItemDelegate *m_delegate;
    Plasma::ScrollBar *m_scrollBar;
    QStyleOptionViewItemV4 m_option;
    QModelIndex m_rootIndex;
};

#endif
