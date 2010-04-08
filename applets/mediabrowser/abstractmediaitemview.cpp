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
#include <KFileItem>
#include <KDirModel>
#include <KDirLister>
#include <QPainter>
#include <KIconLoader>
#include <KGlobalSettings>
#include <KDebug>

#include <Nepomuk/Resource>

// Qt
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QLocale>
#include <QGraphicsSceneHoverEvent>
#include <QApplication>

AbstractMediaItemView::AbstractMediaItemView(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_model(0),
m_scrollBar(new Plasma::ScrollBar(this)),
m_scrollMode(PerPixel),
m_hoveredItem(0),
m_blurred(true)

{
    setFlags(QGraphicsItem::ItemClipsChildrenToShape | QGraphicsItem::ItemIsFocusable);
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
        disconnect(m_model);
        reset();
        delete m_model;
    }

    m_model = model;
    m_rootIndex = QModelIndex();
    if (!m_model) {
        return;
    }

    generateItems(m_rootIndex, 0, model->rowCount() - 1);
    connect (m_model, SIGNAL(modelAboutToBeReset()), this, SLOT(reset()));
    connect (m_model, SIGNAL(rowsInserted(const QModelIndex & , int, int)), this, SLOT(generateItems(const QModelIndex&, int, int)));
    connect (m_model, SIGNAL(rowsRemoved(const QModelIndex &, int, int)), this, SLOT(removeItems(const QModelIndex&, int, int)));
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
    contents.setWidth(contents.width() - m_scrollBar->size().width());

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

void AbstractMediaItemView::updateHoveredItem(ViewItem *item)
{
    if (!item || !m_items.contains(item)) {
        m_hoverIndicator->setPos(0, -100);
        if (m_hoverIndicator->m_nepomuk) {
            m_hoverIndicator->updateHoverRating(QPoint(-1, -1));
        }
        if (m_hoveredItem) {
            update();
        }
        m_hoveredItem = 0;
        return;
    }

    m_hoveredItem = item;
    m_hoverIndicator->m_nepomuk = item->m_nepomuk;
    Plasma::Animator::self()->moveItem(m_hoverIndicator, Plasma::Animator::SlideInMovement, m_hoveredItem->pos().toPoint());
}

void AbstractMediaItemView::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(itemFromPos(event->pos()));
    if (m_hoveredItem) {
        m_hoverIndicator->m_rating = m_hoveredItem->m_nepomuk ? m_hoveredItem->m_resource->rating() : 0;
    }

    if (m_hoverIndicator->m_nepomuk) {
        m_hoverIndicator->updateHoverRating(mapToItem(m_hoverIndicator, event->pos()).toPoint());
    }
}

void AbstractMediaItemView::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    updateHoveredItem(itemFromPos(event->pos()));
    if (m_hoveredItem) {
        m_hoverIndicator->m_rating = m_hoveredItem->m_nepomuk ? m_hoveredItem->m_resource->rating() : 0;
    }
    if (m_hoverIndicator->m_nepomuk) {
        m_hoverIndicator->updateHoverRating(mapToItem(m_hoverIndicator, event->pos()).toPoint());
    }
}

void AbstractMediaItemView::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	Q_UNUSED(event);
    updateHoveredItem(0);
}

void AbstractMediaItemView::setRating(int rating)
{
    if (!m_hoveredItem) {
        return;
    }

    m_hoveredItem->m_resource->setRating(rating);
}

QModelIndex AbstractMediaItemView::indexFromPos(const QPointF &pos)
{
    ViewItem *item = itemFromPos(pos);
    if (item) {
        return item->index();
    }
    return QModelIndex();
}

ViewItem* AbstractMediaItemView::itemFromPos(const QPointF &pos)
{
    foreach (ViewItem *item, m_items) {
        if (item->geometry().contains(pos)) {
            return item;
        }
    }

    return 0;
}

void AbstractMediaItemView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    setFocus(Qt::OtherFocusReason);
}

void AbstractMediaItemView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) {
        return;
    }

    if ((event->pos() - event->buttonDownPos(Qt::LeftButton)).toPoint().manhattanLength()
        >= QApplication::startDragDistance()) {
        tryDrag(event);
    }
}

void AbstractMediaItemView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (KGlobalSettings::singleClick()) {
        itemClickEvent(event);
    }
}

void AbstractMediaItemView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (KGlobalSettings::singleClick()) {
        return;
    }

    itemClickEvent(event);
}

void AbstractMediaItemView::itemClickEvent(QGraphicsSceneMouseEvent *event)
{
    ViewItem *viewItem = itemFromPos(event->pos());
    if (!viewItem) {
        return;
    }


    QModelIndex index = viewItem->index();

    KDirModel *model = qobject_cast<KDirModel*>(m_model);
    if (model && model->dirLister()->isFinished()) {
        KFileItem item = index.isValid() ? index.data(KDirModel::FileItemRole).value<KFileItem>() : KFileItem();
        if (item.isNull()) {
            return;
        }

        if (item.isDir()) {
            model->dirLister()->stop();
            m_history << model->dirLister()->url();
            model->dirLister()->openUrl(item.url());
            m_hoveredItem = 0;
            m_rootIndex = model->indexForItem(item);
	    emit mediasListInDirectory(listMediaInDirectory()); //FIXME: This doesn't seem to work. m_items is always empty?
        } else {

            MediaCenter::Media media;
            media.first = MediaCenter::getType(item.url().path());
            media.second = item.url().path();
	    emit mediasActivated(QList<MediaCenter::Media>() << media);
	    emit mediaActivated(media);
        }
    }

    emit indexActivated(index);
}

//FIXME: This doesnt work. I wanted to have this to be able to have a picture slideshow of all media in the current directory
QList<MediaCenter::Media> AbstractMediaItemView::listMediaInDirectory()
{
    MediaCenter::Media media;
    QList<MediaCenter::Media> list;
    KFileItem item;
    KDirModel *model = qobject_cast<KDirModel*>(m_model);

    for (int i = 0; i < m_items.count(); i++) {
      QModelIndex index = m_items[i]->index();
      if (model && model->dirLister()->isFinished()) {
        item = index.isValid() ? index.data(KDirModel::FileItemRole).value<KFileItem>() : KFileItem();
        //if (item.isNull()) {
        //    return;
        //}
      }
      media.first = MediaCenter::getType(item.url().path());
      media.second = item.url().path();
      list << media;
    }
    kWarning() << "m_items: " << m_items.size() << "created mediaDirList " << list.size();
    return list;
}

void AbstractMediaItemView::tryDrag(QGraphicsSceneMouseEvent *event)
{
    QDrag *drag = new QDrag(event->widget());
    QMimeData *mime = new QMimeData;

    // NOTE: for drag to work a KDirModel::FileItemRole is needed.
    //       if none is found no drag can occur.
    ViewItem *viewItem = itemFromPos(event->pos());
    if (!viewItem) {
        return;
    }

    QModelIndex index = viewItem->index();
    if (!index.isValid()) {
        return;
    }
    KFileItem item = index.data(KDirModel::FileItemRole).value<KFileItem>();
    if (item.isNull()) {
        return;
    }
    mime->setUrls(QList<QUrl>() << item.url());
    mime->setText(item.url().pathOrUrl());
    drag->setMimeData(mime);

    QPixmap pixmap(viewItem->size().toSize());
    pixmap.fill(Qt::transparent);
    QPainter p(&pixmap);

    QStyleOptionGraphicsItem o;
    o.rect = pixmap.rect();

    viewItem->paint(&p, &o, 0);

    drag->setPixmap(pixmap);

    drag->exec(Qt::MoveAction);
}

void AbstractMediaItemView::goPrevious()
{
    if (m_history.isEmpty()) {
        return;
    }

    KDirModel *model = qobject_cast<KDirModel*>(this->model());
    if (!model) {
        return;
    }

    if (!m_history.last().isValid()) {
        return;
    }
    model->dirLister()->stop();
    model->dirLister()->openUrl(m_history.last());
    m_rootIndex = model->indexForUrl(m_history.last());
    m_history.removeLast();
}

void AbstractMediaItemView::setDrawBlurredText(bool set)
{
    if (m_blurred == set) {
        return;
    }

    kDebug() << "setting blurred text to" << set;
    m_blurred = set;
    foreach (ViewItem *item, m_items) {
        item->setDrawBlurredText(set);
    }
}

bool AbstractMediaItemView::drawBlurredText()
{
    return m_blurred;
}

void AbstractMediaItemView::reset()
{
    qDeleteAll(m_items);
    m_items.clear();
    m_hoveredItem = 0;
}

void AbstractMediaItemView::removeItems(const QModelIndex &parent, int start, int end)
{
    if (m_rootIndex != parent) {
        return;
    }

    if (start < 0 || end >= m_items.count()) {
        return;
    }
    m_hoveredItem = 0;
    QList<ViewItem*>::iterator startIt = m_items.begin();
    QList<ViewItem*>::iterator endIt = m_items.begin();
    startIt += (start);
    endIt += (end);
    for (; startIt <= endIt; ++startIt) {
        delete *startIt;
        m_items.erase(startIt);
    }
    layoutItems();
    updateScrollBar();
}

void AbstractMediaItemView::setBrowsingType(ModelPackage::BrowsingType type)
{
	m_browsingType = type;
}

ModelPackage::BrowsingType AbstractMediaItemView::browsingType()
{
	return m_browsingType;
}