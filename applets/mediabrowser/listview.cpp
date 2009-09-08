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
#include "listview.h"
#include "viewitem.h"

// Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDir>
#include <QScrollBar>
#include <QGraphicsSceneResizeEvent>
#include <QGraphicsSceneHoverEvent>
#include <QDrag>
#include <QApplication>
#include <QMimeData>
#include <QKeyEvent>

// KDE
#include <KDirModel>
#include <KDirLister>
#include <KUrl>
#include <KFileItem>
#include <KDebug>

// Plasma
#include <Plasma/Animator>

ListView::ListView(QGraphicsItem *parent) : AbstractMediaItemView(parent)
{
    setupOptions();
}

ListView::~ListView()
{}

void ListView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsWidget::paint(painter, option, widget);
}

void ListView::setupOptions()
{
    AbstractMediaItemView::setupOptions();
    m_option.decorationPosition = QStyleOptionViewItem::Left;
    m_option.decorationAlignment = Qt::AlignCenter;
    m_option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
}

void ListView::layoutItems()
{
    const int x = contentsArea().x();
    int y = contentsArea().y();
    const int width = contentsArea().width();

    for (int i = 0; i < m_items.count(); i++) {
//        if (y > contentsArea().bottom()) { // TODO: do not even create hidden items
//            return;
//        }
        if (i == 0) {
            y -= m_scrollMode == AbstractMediaItemView::PerItem ? (verticalScrollBar()->value() * m_items[i]->size().height())
                                  : verticalScrollBar()->value();
        }
        m_items[i]->setPos(x, y);
        m_items[i]->resize(width, m_items[i]->itemSizeHint().height());
        y += m_items[i]->size().height();
        m_hoverIndicator->resize(width, m_items[i]->itemSizeHint().height());
    }

    if (m_hoveredItem) {
        m_hoverIndicator->setPos(m_hoveredItem->pos());
    }
}

void ListView::generateItems()
{
    qDeleteAll(m_items);
    m_items.clear();
    for (int i = 0; i < m_model->rowCount(m_rootIndex); i++) {
        kDebug() << "begin of cycle";
        ViewItem *item = new ViewItem(m_option, this);
        kDebug() << "setting model index to the item";
        item->setModelIndex(m_model->index(i, 0, m_rootIndex));
        item->setDrawBlurredText(m_blurred);
        m_items << item;
    }
    layoutItems();
}

void ListView::updateScrollBar()
{
    if (m_scrollMode == PerItem) {
        verticalScrollBar()->setRange(0, m_model->rowCount(m_rootIndex));
        verticalScrollBar()->setSingleStep(1);
    } else {
        verticalScrollBar()->setRange(0, m_model->rowCount(m_rootIndex) * iconSize() * 2 - rect().height());
        verticalScrollBar()->setSingleStep(m_model->rowCount(m_rootIndex));
        verticalScrollBar()->setPageStep(iconSize() * 2);
    }
}

void ListView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    AbstractMediaItemView::resizeEvent(event);

    if (m_model) {
        updateScrollBar();
    }
}

void ListView::keyPressEvent(QKeyEvent *event)
{
    if (m_items.isEmpty()) {
        return;
    }

    if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down) {
        if (!m_hoveredItem) {
            updateHoveredItem(m_items.first());
        } else {
            int index = m_items.indexOf(m_hoveredItem);
            if (index == -1) {
                // We should not be here
                return;
            }
            if (event->key() == Qt::Key_Up) {
                --index;
            } else {
                ++index;
            }

            if (index < 0 || index >= m_items.count()) {
                return;
            }
            updateHoveredItem(m_items.at(index));
        }
    }

}
