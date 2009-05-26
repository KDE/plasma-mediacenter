/*
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "activeitemwidget.h"

#include <QIcon>
#include <QVariant>
#include <QHash>
#include <QList>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QTimeLine>

#include <KDebug>

static const int DURATION = 250; // ms

class ActiveItemWidget::ActiveItemWidgetPrivate
{
public:
    ActiveItemWidgetPrivate(ActiveItemWidget *q) : q(q), activeItem(0), iconSize(0), activeIndex(-1), xOffset(0.0), xPress(0.0), timeLine(new QTimeLine(DURATION, q))
    {}
    ~ActiveItemWidgetPrivate()
    {}

    ActiveItemWidget *q;
    QList<Item*> items;
    Item *activeItem;
    int iconSize;
    int activeIndex;

    qreal xOffset;
    qreal xPress;

    QTimeLine *timeLine;

    void paintItem(Item *, QPainter *, const QStyleOptionGraphicsItem *);
    void setActiveItem(Item *);

};

ActiveItemWidget::ActiveItemWidget(QGraphicsItem *parent) : QGraphicsWidget(parent), d(new ActiveItemWidgetPrivate(this))
{
    connect (d->timeLine, SIGNAL(valueChanged(qreal)), this, SLOT(animateScroll(qreal)));
}

ActiveItemWidget::~ActiveItemWidget()
{
    delete d;
}

void ActiveItemWidget::addItem(const QString &text)
{
    Item *item = new Item;
    item->text = text;

    d->items.append(item);

    if (d->items.count() == 1) {
        d->setActiveItem(item);
        update();
    }
}

void ActiveItemWidget::addItem(const QIcon &icon, const QString &text)
{
    Item *item = new Item;
    item->text = text;
    item->icon = icon;

    d->items.append(item);

    if (d->items.count() == 1) {
        d->setActiveItem(item);
        update();
    }
}

void ActiveItemWidget::addItems(const QStringList &items)
{
    foreach (const QString &text, items) {
        addItem(text);
    }
}

int ActiveItemWidget::count() const
{
    return d->items.count();
}

int ActiveItemWidget::activeItemIndex() const
{
    return d->items.indexOf(d->activeItem);
}

void ActiveItemWidget::setIconSize(int size)
{
    d->iconSize = size;
}

int ActiveItemWidget::iconSize() const
{
    return d->iconSize;
}

QIcon ActiveItemWidget::itemIcon(int index) const
{
    return d->items[index]->icon;
}

QString ActiveItemWidget::itemText(int index) const
{
    return d->items[index]->text;
}

QVariant ActiveItemWidget::itemData(int index, int role) const
{
    return d->items[index]->data[role];
}

QIcon ActiveItemWidget::activeItemIcon() const
{
    return d->activeItem->icon;
}

QString ActiveItemWidget::activeItemText() const
{
    return d->activeItem->text;
}

QVariant ActiveItemWidget::activeItemData(int role) const
{
    return d->activeItem->data[role];
}

void ActiveItemWidget::setItemIcon(int index, const QIcon &icon)
{
    if (index < 0 || index >= d->items.count()) {
        return;
    }

    Item *item = d->items[index];
    item->icon = icon;

    if (item == d->activeItem) {
        update();
    }
}

void ActiveItemWidget::setItemText(int index, const QString &text)
{
    if (index < 0 || index >= d->items.count()) {
        return;
    }

    Item *item = d->items[index];
    item->text = text;

    if (item == d->activeItem) {
        update();
    }
}

void ActiveItemWidget::setItemData(int index, const QVariant &value, int role)
{
    if (index < 0 || index >= d->items.count()) {
        return;
    }

    Item *item = d->items[index];
    item->data[role] = value;

    if (item == d->activeItem) {
        update();
    }
}

void ActiveItemWidget::next()
{
    int current = d->items.indexOf(d->activeItem);

    // could this ever happen?
    if (current == -1) {
        return;
    }

    ++current;
    if (current == d->items.count()) {
        return;
    }

    d->setActiveItem(d->items[current]);
    emit activeItemChanged(current);

    update();
}

void ActiveItemWidget::previous()
{
    int current = d->items.indexOf(d->activeItem);

    // could this ever happen?
    if (current == -1) {
        return;
    }

    --current;
    if (current < 0) {
        return;
    }

    d->setActiveItem(d->items[current]);
    emit activeItemChanged(current);

    update();
}

void ActiveItemWidget::setActiveItem(int index)
{
    if (index < 0 || index >= d->items.count()) {
        return;
    }

    d->setActiveItem(d->items[index]);
    emit activeItemChanged(index);

    update();
}

void ActiveItemWidget::setActiveItemText(const QString &text)
{
    if (!d->activeItem) {
        return;
    }

    d->activeItem->text = text;
    emit activeItemTextChanged();

    update();
}

void ActiveItemWidget::setActiveItemIcon(const QIcon &icon)
{
    if (!d->activeItem) {
        return;
    }

    d->activeItem->icon = icon;
    emit activeItemIconChanged();

    update();
}

void ActiveItemWidget::setActiveItemData(const QVariant &value, int role)
{
    if (!d->activeItem) {
        return;
    }

    d->activeItem->data[role] = value;
    emit activeItemDataChanged(role);

    update();
}

void ActiveItemWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    kDebug() << d->xOffset;
    painter->save();
    painter->setClipRect(option->rect);

    // let's move to the current item
    kDebug() << d->activeIndex;
    if (d->activeIndex > -1) {
        painter->translate(d->activeIndex * option->rect.width(), 0);
    }

    // let's follow mouse/digit gesture
    painter->translate(d->xOffset, 0);

    qreal animatedValue = d->timeLine->currentValue();
    if (d->timeLine->currentFrame() < 0) {
        animatedValue = -animatedValue;
    }
    kDebug() << "translating" << animatedValue;
    painter->translate(animatedValue * (option->rect.width() - d->xOffset), 0);

    kDebug() << d->items.count();
    foreach (Item *item, d->items) {
        kDebug() << "calling paint item";
        d->paintItem(item, painter, option);
        painter->translate(option->rect.width(), 0);
    }
    painter->restore();
}

QSizeF ActiveItemWidget::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
//    Q_UNUSED(which)
//    Q_UNUSED(constraint)
//
    if (which == Qt::PreferredSize) {
        if (!d->activeItem) {
            return QGraphicsWidget::sizeHint(which, constraint);
        }
        return QSizeF(d->iconSize + qApp->fontMetrics().width(d->activeItem->text),
                      qMax(d->iconSize, qApp->fontMetrics().height()));
    }

    return QGraphicsWidget::sizeHint(which, constraint);
}

void ActiveItemWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    d->xPress = event->pos().x();
}

void ActiveItemWidget::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    d->xOffset = event->pos().x() - d->xPress;
    update();
}

void ActiveItemWidget::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (abs(d->xOffset) > contentsRect().width() / 4) {
        int xLeft = contentsRect().width() - abs(d->xOffset);
        if (d->xOffset < 0) {
            d->timeLine->setFrameRange(0, -xLeft);
        } else {
            d->timeLine->setFrameRange(0, xLeft);
        }
        d->timeLine->start();
        return;
    }

    d->xOffset = 0.0;
    d->xPress = 0.0;
    update();
}

void ActiveItemWidget::animateScroll(qreal value)
{
    Q_UNUSED(value)
    update();
}

void ActiveItemWidget::ActiveItemWidgetPrivate::paintItem(Item *item, QPainter *painter, const QStyleOptionGraphicsItem *option)
{
    if (!item->icon.isNull()) {
        item->icon.paint(painter, QRect(option->rect.topLeft(), QSize(iconSize, iconSize)));
    }

    QRect textRect = option->rect;
    textRect.setSize(QSize(textRect.width() - iconSize, textRect.height()));
    textRect.translate(iconSize, 0);

    kDebug() << "drawing" << item->text;
    painter->drawText(textRect, Qt::AlignCenter, item->text);
}

void ActiveItemWidget::ActiveItemWidgetPrivate::setActiveItem(Item *item)
{
    if (!items.contains(item)) {
        return;
    }

    activeIndex = items.indexOf(item);
    activeItem = item;
}
