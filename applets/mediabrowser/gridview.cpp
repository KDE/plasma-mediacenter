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
#include "gridview.h"
#include "viewitem.h"

#include <QScrollBar>
#include <QTimer>

#include <Plasma/Animator>

GridView::GridView(QGraphicsItem *parent) : AbstractMediaItemView(parent),
m_itemLines(0),
m_timer(new QTimer(this)),
m_lastHoveredItem(0)
{
    setupOptions();
    m_timer->setInterval(2000);

    connect (m_timer, SIGNAL(timeout()), this, SLOT(highlightHoveredItem()));
//    connect (Plasma::Animator::self(), SIGNAL(animationFinished(QGraphicsItem*,Plasma::Animator::Animation)),
//             this, SLOT(slotAnimationFinished(QGraphicsItem*,Plasma::Animator::Animation)));
}

GridView::~GridView()
{}

void GridView::setupOptions()
{
    AbstractMediaItemView::setupOptions();
    m_option.decorationPosition = QStyleOptionViewItem::Top;
    m_option.decorationAlignment = Qt::AlignCenter;
    m_option.displayAlignment = Qt::AlignCenter;
}

void GridView::layoutItems()
{
    int x = contentsArea().x();
    int y = contentsArea().y();
    const int width = contentsArea().width();
    m_itemLines = 0;

    for (int i = 0; i < m_items.count(); i++) {
        if (i == 0) {
            y -= m_scrollMode == AbstractMediaItemView::PerItem ? (verticalScrollBar()->value() * m_items[i]->size().height())
                                  : verticalScrollBar()->value();
        }
        m_items[i]->setPos(x, y);
        m_items[i]->resize(m_items[i]->itemSizeHint());

        if (x + (m_items[i]->size().width() * 2) > contentsArea().right()) {
           y += m_items[i]->size().height();
           x = 0;
           ++m_itemLines;
         } else {
           x += m_items[i]->size().width();
         }

         m_hoverIndicator->resize(m_items[i]->size()); // TODO: no need to iterate this
    }

    if (m_hoveredItem) {
        m_hoverIndicator->setPos(m_hoveredItem->pos());
    }
}

void GridView::generateItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    for (int i = 0; i < m_model->rowCount(m_rootIndex); i++) {
        ViewItem *item = new ViewItem(m_option, this);
        item->setModelIndex(m_model->index(i, 0, m_rootIndex));
        m_items << item;
    }
    layoutItems();
}

void GridView::updateScrollBar()
{
    if (m_items.isEmpty()) {
        return;
    }
    if (m_scrollMode == PerItem) {
        verticalScrollBar()->setRange(0, m_itemLines);
        verticalScrollBar()->setSingleStep(1);
    } else {
        verticalScrollBar()->setRange(0, m_itemLines * m_items[0]->size().height() - rect().height());
        verticalScrollBar()->setSingleStep(1);
        verticalScrollBar()->setPageStep(iconSize() * 2);
    }
}

void GridView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    AbstractMediaItemView::resizeEvent(event);

    if (m_model) {
        updateScrollBar();
    }
}

void GridView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    AbstractMediaItemView::hoverEnterEvent(event);

    if (m_hoveredItem) {
        m_lastHoveredItem = m_hoveredItem;
        m_timer->start();
    }
}

void GridView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    AbstractMediaItemView::hoverMoveEvent(event);

    if (m_hoveredItem && m_hoveredItem != m_lastHoveredItem) {
        m_lastHoveredItem = m_hoveredItem;
        m_timer->stop();
        layoutItems();
        m_timer->start();
    }
}

void GridView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    AbstractMediaItemView::hoverLeaveEvent(event);

    if (m_timer->isActive()) {
        m_timer->stop();
    }
    m_lastHoveredItem = 0;
}

void GridView::highlightHoveredItem()
{
    Plasma::Animator::self()->customAnimation(100, 250, Plasma::Animator::EaseInCurve, this, "highlightAnimation");
    m_hoveredItem->setZValue(1000);
    m_timer->stop();
}

void GridView::highlightAnimation(qreal value)
{
    m_hoveredItem->resize( (m_hoveredItem->itemSizeHint().width() * 2) * value, (m_hoveredItem->itemSizeHint().height() * 2) * value);
    m_hoverIndicator->resize(m_hoveredItem->size());
}

QModelIndex GridView::indexFromPos(const QPointF &pos)
{
    foreach (ViewItem *item, m_items) {
        if (item->rect().contains(pos)) {
            return item->index();
        }
    }

    return QModelIndex();
}
