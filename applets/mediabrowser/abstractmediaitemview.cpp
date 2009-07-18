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
#include "abstractmediaitemview.h"
#include "viewitem.h"

// Plasma
#include <Plasma/ScrollBar>
#include <Plasma/Theme>
#include <Plasma/Animator>

// KDE
#include <KIconLoader>
#include <KDebug>

#include <Nepomuk/Resource>

// Qt
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QLocale>
#include <QGraphicsSceneHoverEvent>

AbstractMediaItemView::AbstractMediaItemView(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_model(0),
m_scrollBar(new Plasma::ScrollBar(this)),
m_scrollMode(PerPixel),
m_hoveredItem(0)

{
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setAcceptsHoverEvents(true);
    setIconSize(KIconLoader::global()->currentSize(KIconLoader::Desktop));
    connect (m_scrollBar, SIGNAL(valueChanged(int)), this, SLOT(layoutItems()));
    m_scrollBar->setZValue(1000);

    QStyleOptionViewItemV4 opt = m_option;
    opt.state |= QStyle::State_MouseOver;
    m_hoverIndicator = new ViewItem(opt, this);
    m_hoverIndicator->setZValue(-1000);
    m_hoverIndicator->setPos(0, -100);

    m_hoverIndicator->setAcceptedMouseButtons(Qt::LeftButton);
    connect (m_hoverIndicator, SIGNAL(ratingActivated(int)), this, SLOT(setRating(int)));
}

AbstractMediaItemView::~AbstractMediaItemView()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void AbstractMediaItemView::setIconSize(int size)
{
    m_iconSize = size;
    update();
}

int AbstractMediaItemView::iconSize() const
{
    return m_iconSize;
}

void AbstractMediaItemView::setModel(QAbstractItemModel *model)
{
    if (m_model) {
        delete m_model;
    }

    m_model = model;
//    connect (m_model, SIGNAL(modelReset()), this, SLOT(generateItems()));
}

QAbstractItemModel* AbstractMediaItemView::model()
{
    return m_model;
}

QScrollBar* AbstractMediaItemView::verticalScrollBar()
{
    return m_scrollBar->nativeWidget();
}

QRect AbstractMediaItemView::contentsArea() const
{
    if (!m_scrollBar->isVisible()) {
        return contentsRect().toRect();
    }
    QRect contents = contentsRect().toRect();
    contents.setWidth(contents.width() - m_scrollBar->nativeWidget()->width());

    return contents;
}

void AbstractMediaItemView::hideVerticalScrollBar()
{
    m_scrollBar->hide();
}

bool AbstractMediaItemView::isVerticalScrollBarHidden()
{
    return !m_scrollBar->isVisible();
}

void AbstractMediaItemView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    m_scrollBar->resize(m_scrollBar->size().width(), contentsRect().height());
    m_scrollBar->setPos(contentsRect().width() - m_scrollBar->size().width(), 0);

    layoutItems();

}

void AbstractMediaItemView::setupOptions()
{
    m_option.palette.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    m_option.font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DesktopFont);
    m_option.fontMetrics = QFontMetrics(m_option.font);
    m_option.decorationSize = QSize(iconSize(), iconSize());
    m_option.locale = QLocale::system();
    m_option.widget = 0;
    m_option.state |= QStyle::State_Enabled;
}

void AbstractMediaItemView::setRootIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

QModelIndex AbstractMediaItemView::rootIndex() const
{
    return m_rootIndex;
}

void AbstractMediaItemView::setScrollMode(ScrollMode mode)
{
    m_scrollMode = mode;
}

AbstractMediaItemView::ScrollMode AbstractMediaItemView::scrollMode()
{
    return m_scrollMode;
}

void AbstractMediaItemView::invalidate()
{
    qDeleteAll(m_items);
    m_items.clear();
}

void AbstractMediaItemView::updateHoveredItem(const QPointF &point)
{
    for (int i = 0; i < m_items.count(); i++) {
        if (m_items[i]->rect().contains(mapToItem(m_items[i], point)) && m_items[i] != m_hoveredItem) {

            m_hoveredItem = m_items[i];
            Plasma::Animator::self()->moveItem(m_hoverIndicator, Plasma::Animator::SlideInMovement, m_hoveredItem->pos().toPoint());

            break;
        }
    }
}

void AbstractMediaItemView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(event->pos());
    if (m_hoveredItem) {
        m_hoverIndicator->m_rating = m_hoveredItem->m_resource->rating();
    }
    m_hoverIndicator->updateHoverRating(mapToItem(m_hoverIndicator, event->pos()).toPoint());
}

void AbstractMediaItemView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(event->pos());
    if (m_hoveredItem) {
        m_hoverIndicator->m_rating = m_hoveredItem->m_resource->rating();
    }
    m_hoverIndicator->updateHoverRating(mapToItem(m_hoverIndicator, event->pos()).toPoint());
}

void AbstractMediaItemView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_hoverIndicator->setPos(0, -100);
    m_hoveredItem = 0;
    m_hoverIndicator->updateHoverRating(mapToItem(m_hoverIndicator, event->pos()).toPoint());
    update();
}

void AbstractMediaItemView::setRating(int rating)
{
    if (!m_hoveredItem) {
        return;
    }

    m_hoveredItem->m_resource->setRating(rating);
}
