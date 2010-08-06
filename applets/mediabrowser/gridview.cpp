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
#include "fileviewitem.h"

#include <QTimer>
#include <QKeyEvent>

#include <KDebug>

#include <Plasma/Animator>
#include <Plasma/ScrollBar>

GridView::GridView(QGraphicsItem *parent) : AbstractMediaItemView(parent),
m_itemRows(0),
m_timer(new QTimer(this)),
m_lastHoveredItem(0),
m_highlighting(false)
{
    setupOptions();
    m_timer->setInterval(2000);

//    connect (m_timer, SIGNAL(timeout()), this, SLOT(highlightHoveredItem()));
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
    m_option.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;
}

void GridView::layoutItems()
{
    int x = contentsArea().x();
    int y = contentsArea().y();
    const int width = contentsArea().width();
    m_itemRows = 0;

    for (int i = 0; i < m_items.count(); i++) {
        if (i == 0) {
            y -= m_scrollMode == AbstractMediaItemView::PerItem ? (verticalScrollBar()->value() * m_items[i]->size().height())
                                  : verticalScrollBar()->value();
        }
        m_items[i]->setPos(x, y);
        m_items[i]->resize(m_items[i]->itemSizeHint());

        if ((x + m_items[i]->size().width() * 2) > contentsArea().width()) {
           y += m_items[i]->size().height();
           x = 0;
           ++m_itemRows;
         } else {
           x += m_items[i]->size().width();
         }
    }

    if (m_hoveredItem) {
        m_hoverIndicator->resize(m_hoveredItem->size());
    }
}

void GridView::generateItems(const QModelIndex &parent, int start, int end)
{
    if (parent != m_rootIndex) {
        return;
    }

    for (int i = start; i <= end; i++) {
        FileViewItem *item = new FileViewItem(m_option, this);
        item->setModelIndex(m_model->index(i, 0, m_rootIndex));
        item->setDrawBlurredText(m_blurred);
        m_items.insert(i, item);
    }
    layoutItems();
    updateScrollBar();
}

void GridView::updateScrollBar()
{
    if (m_items.isEmpty()) {
        return;
    }
    if (m_scrollMode == PerItem) {
        verticalScrollBar()->setRange(0, m_itemRows);
        verticalScrollBar()->setSingleStep(1);
    } else {
        verticalScrollBar()->setRange(0, m_itemRows * m_items[0]->size().height() - rect().height());
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
//
//    if (m_hoveredItem) {
//        m_lastHoveredItem = m_hoveredItem;
//        m_timer->start();
//    }
}

void GridView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    AbstractMediaItemView::hoverMoveEvent(event);

//    if (m_hoveredItem && m_hoveredItem != m_lastHoveredItem) {
//        m_lastHoveredItem = m_hoveredItem;
//        m_timer->stop();
//        if (m_highlighting) {
//            layoutItems();
//            restoreItems();
//            m_highlighting = false;
//        }
//        m_timer->start();
//    }
}

void GridView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    AbstractMediaItemView::hoverLeaveEvent(event);

//    if (m_timer->isActive()) {
//        m_timer->stop();
//        m_highlighting = false;
//    }
//    m_lastHoveredItem = 0;
}

void GridView::highlightHoveredItem()
{
    fadeOutItems(m_hoveredItem);
    Plasma::Animator::self()->customAnimation(100, 250, Plasma::Animator::EaseInCurve, this, "highlightAnimation");
    m_hoveredItem->setZValue(1000);
    m_timer->stop();
}

void GridView::highlightAnimation(qreal value)
{
    m_highlighting = true;
    m_hoveredItem->resize( (m_hoveredItem->itemSizeHint().width() * 2) * value, (m_hoveredItem->itemSizeHint().height() * 2) * value);
    m_hoverIndicator->resize(m_hoveredItem->size());
}

void GridView::fadeOutItems(FileViewItem *exception)
{
    for (int i = 0; i < m_items.count(); i++) {
        if (m_items[i] != exception) {
            m_items[i]->setOpacity(0.3);
        }
    }
}

void GridView::restoreItems()
{
    for (int i = 0; i < m_items.count(); i++) {
        m_items[i]->setOpacity(1);
    }
}

void GridView::keyPressEvent(QKeyEvent *event)
{
    if (m_items.isEmpty()) {
        return;
    }

    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right) {
        if (!m_hoveredItem) {
            updateHoveredItem(m_items.first());
        } else {
            int index = m_items.indexOf(m_hoveredItem);
            if (index == -1) {
                // We should not be here
                return;
            }
            if (event->key() == Qt::Key_Left) {
                --index;
            } else { // Qt::Key_Right
                ++index;
            }

            if (index < 0 || index >= m_items.count()) {
                return;
            }
            updateHoveredItem(m_items.at(index));
        }
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        if (!m_hoveredItem) {
            updateHoveredItem(m_items.first());
        } else {
            QPointF currentPoint = m_hoveredItem->pos() + QPointF(1, 1);
            event->key() == Qt::Key_Up ? currentPoint.setY(currentPoint.y() - m_hoveredItem->size().height()) :
                                        currentPoint.setY(currentPoint.y() + m_hoveredItem->size().height());
            FileViewItem *newItem = itemFromPos(currentPoint);
            if (newItem) {
                updateHoveredItem(newItem);
            } else {
                updateHoveredItem(m_items.last());
            }
        }
    } else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
        QPointF currentPoint = m_hoveredItem->pos() + QPointF(1, 1);
        itemActivateEvent(itemFromPos(currentPoint));
    }
}
