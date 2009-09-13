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
#include <KUrl>
#include <mediacenter/mediacenter.h>

class QAbstractItemModel;

namespace Plasma {
    class ScrollBar;
}
class ViewItem;
class QScrollBar;

class AbstractMediaItemView : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum ScrollMode { PerItem, PerPixel };

    AbstractMediaItemView(QGraphicsItem *parent = 0);
    virtual ~AbstractMediaItemView();

    void setIconSize(int size);
    int iconSize() const;

    /**
     * Always set model through this method, even if m_model is available
     * as protected.
     */
    virtual void setModel(QAbstractItemModel *model);
    QAbstractItemModel *model();

    void setRootIndex(const QModelIndex &);
    QModelIndex rootIndex() const;

    QScrollBar *verticalScrollBar();

    void hideVerticalScrollBar();
    bool isVerticalScrollBarHidden();

    QRect contentsArea() const;

    void setScrollMode(ScrollMode);
    ScrollMode scrollMode();

    void setDrawBlurredText(bool set);
    bool drawBlurredText();

    /**
     * Call this method whenever events like model resetting occur.
     * This method destroys all the existing items so only call it whenever
     * you are sure the current view has to be invalidated.
     */
    void invalidate();

    QModelIndex indexFromPos(const QPointF &);

protected:
    virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
    /**
     * Reimplement this method in order to fill in m_option. Remember
     * to always call the base implementation since it already initializes
     * m_option with the basic stuff. All you need to do is specify things
     * specific for the kind of view you are going to implement (icon view or list view).
     */
    virtual void setupOptions();

    virtual void updateHoveredItem(ViewItem *item);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    void reset();

protected slots:
    /**
     * You must reimplement this method in order to always
     * keep up to date your items' position and size. Set size and
     * position in this method. It'll be called as needed.
     */
    virtual void layoutItems() = 0;

public slots:
    /**
     * Reimplement this slot in order to put here
     * the code for items generations. Items must be collected
     * in the m_items list. You just need to call this once for
     * each rootIndex change in the model.
     */
    virtual void generateItems() = 0;

    /**
     * Reimplement this function in order to update
     * ranges and steps of the vertical scroll bar. Call
     * it whenever the row count changes in order to update
     * the scrollbar.
     */
    virtual void updateScrollBar() = 0;
    
    void goPrevious();

private slots:
    void setRating(int);

signals:
    void scrollOffsetChanged(int);
    void mediasActivated(const QList<MediaCenter::Media> &);
    void indexActivated(const QModelIndex &);

private:
    void tryDrag(QGraphicsSceneMouseEvent *);
    ViewItem* itemFromPos(const QPointF &);
    void itemClickEvent(QGraphicsSceneMouseEvent *);
    KUrl::List m_history;

protected:
    int m_iconSize;
    QAbstractItemModel *m_model;
    Plasma::ScrollBar *m_scrollBar;
    QStyleOptionViewItemV4 m_option;
    QModelIndex m_rootIndex;
    ScrollMode m_scrollMode;
    bool m_blurred;

    QList<ViewItem*> m_items;

    ViewItem *m_hoveredItem;
    ViewItem *m_hoverIndicator;
};

#endif
