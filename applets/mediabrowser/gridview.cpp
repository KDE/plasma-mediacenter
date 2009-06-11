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

GridView::GridView(QGraphicsItem *parent) : AbstractMediaItemView(parent), m_itemLines(0)
{
    setupOptions();
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

        if (x + m_items[i]->size().width() > contentsArea().right()) {
           y += m_items[i]->size().height();
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
