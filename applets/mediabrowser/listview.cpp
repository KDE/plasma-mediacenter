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
        ViewItem *item = new ViewItem(m_option, this);
        item->setModelIndex(m_model->index(i, 0, m_rootIndex));
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
        verticalScrollBar()->setSingleStep(1);
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

QModelIndex ListView::indexFromPos(const QPointF &pos)
{
    foreach (ViewItem *item, m_items) {
        if (item->geometry().contains(pos)) {
            return item->index();
        }
    }

    return QModelIndex();
}

void ListView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void ListView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    kDebug() << "mouseMoveEvent";
    if ((event->pos() - event->buttonDownPos(Qt::LeftButton)).toPoint().manhattanLength()
        >= QApplication::startDragDistance()) {
        tryDrag(event);
    }
}

void ListView::tryDrag(QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;

    // NOTE: for drag to work a KDirModel::FileItemRole is needed.
    //       if none is found no drag can occur.
    QModelIndex index = indexFromPos(event->pos());
    if (!index.isValid()) {
        return;
    }
    KFileItem item = index.data(KDirModel::FileItemRole).value<KFileItem>();
    if (item.isNull()) {
        return;
    }
    mime->setUrls(QList<QUrl>() << item.url());
    drag->setMimeData(mime);
    drag->setPixmap(item.pixmap(m_option.decorationSize.width()));

    drag->exec(Qt::MoveAction);
}
