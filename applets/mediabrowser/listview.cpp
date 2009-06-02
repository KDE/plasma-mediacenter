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

// KDE
#include <KDirModel>
#include <KDirLister>
#include <KUrl>
#include <KFileItemDelegate>
#include <KDebug>

ListView::ListView(QGraphicsItem *parent) : AbstractMediaItemView(parent), m_hoveredRect(QRect())
{
    setupOptions();
    switchToFileModel();
}

ListView::~ListView()
{}

void ListView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsWidget::paint(painter, option, widget);
//    Q_UNUSED(widget)
//    if (!m_delegate) {
//        return;
//    }

//    painter->setClipRect(contentsArea());
//
//    // scrolling code;
////    painter->translate(0, - verticalScrollBar()->value() * iconSize() * 2);
//
//    for (int i = 0; i < m_rects.count(); i++) {
//        m_option.rect = m_rects[i];
//        if (m_option.rect == m_hoveredRect) {
//            m_option.state |= QStyle::State_MouseOver;
//        }
//        m_delegate->paint(painter, m_option, m_model->index(i, 0, m_rootIndex));
//        m_option.state &= ~QStyle::State_MouseOver;
//    }
}

void ListView::setupOptions()
{
    AbstractMediaItemView::setupOptions();
    m_option.decorationPosition = QStyleOptionViewItem::Left;
    m_option.decorationAlignment = Qt::AlignCenter;
    m_option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
}

void ListView::switchToFileModel()
{
    KDirModel *model = new KDirModel(this);

    KDirLister *lister = new KDirLister(this);
    connect (lister, SIGNAL(completed()), this, SLOT(updateScrollBar()));
    connect (lister, SIGNAL(completed()), this, SLOT(generateItems()));

    model->setDirLister(lister);
    lister->openUrl(KUrl(QDir::homePath()));
    setModel(model);
    m_delegate = new KFileItemDelegate(this);


    update();
}

void ListView::layoutItems()
{
    const int x = contentsArea().x();
    const int height = iconSize() * 2; // TODO check this arbitrary size
    int y = contentsArea().y() - (m_scrollMode == AbstractMediaItemView::PerItem ? (verticalScrollBar()->value() * height)
                                  : verticalScrollBar()->value());
    const int width = contentsArea().width();

    for (int i = 0; i < m_items.count(); i++) {
//        if (y > contentsArea().bottom()) { // TODO: do not even create hidden items
//            return;
//        }
        m_items[i]->setPos(x, y);
        m_items[i]->resize(width, height);
        y += height;
    }
}

void ListView::updateHoveredItem(const QPoint &point)
{
//    for (int i = 0; i < m_rects.count(); i++) {
//        if (m_rects[i].contains(point) && m_rects[i] != m_hoveredRect) {
//            m_hoveredRect = m_rects[i];
//            update();
//            break;
//        }
//    }
}

void ListView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(event->pos().toPoint());
}

void ListView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(event->pos().toPoint());
}

void ListView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hoveredRect = QRect();
    update();
}

void ListView::generateItems()
{
    kDebug() << "";
    for (int i = 0; i < m_model->rowCount(m_rootIndex); i++) {
        ViewItem *item = new ViewItem(this);
        item->setModelIndex(m_model->index(i, 0, m_rootIndex));
        item->setStyleOption(m_option);
        item->setItemDelegate(m_delegate);
        m_items << item;
    }
}

void ListView::updateScrollBar()
{
    if (m_scrollMode == PerItem) {
        verticalScrollBar()->setRange(0, m_model->rowCount(m_rootIndex));
        verticalScrollBar()->setSingleStep(1);
    } else {
        verticalScrollBar()->setRange(0, m_model->rowCount(m_rootIndex) * iconSize() * 2);
        verticalScrollBar()->setSingleStep(1);
    }
}
