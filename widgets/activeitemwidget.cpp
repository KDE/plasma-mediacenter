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

class ActiveItemWidget::ActiveItemWidgetPrivate
{
public:
    ActiveItemWidgetPrivate(ActiveItemWidget *q) : q(q), activeItem(0)
    {}
    ~ActiveItemWidgetPrivate()
    {}

    ActiveItemWidget *q;
    QList<Item*> items;
    Item *activeItem;
    QSize iconSize;
};

ActiveItemWidget::ActiveItemWidget(QGraphicsItem *parent) : QGraphicsWidget(parent), d(new ActiveItemWidgetPrivate(this))
{}

ActiveItemWidget::~ActiveItemWidget()
{
    delete d;
}

void ActiveItemWidget::addItem(const QString &text)
{
    Item *item = new Item;
    item->text = text;

    if (d->items.isEmpty()) {
        d->activeItem = item;
        update();
    }

    d->items.append(item);
}

void ActiveItemWidget::addItem(const QIcon &icon, const QString &text)
{
    Item *item = new Item;
    item->text = text;
    item->icon = icon;

    if (d->items.isEmpty()) {
        d->activeItem = item;
        update();
    }

    d->items.append(item);
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

void ActiveItemWidget::setIconSize(const QSize &size)
{
    d->iconSize = size;
}

QSize ActiveItemWidget::iconSize() const
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

QVariant ActiveItemWidget::itemData(int index, int role = Qt::UserRole) const
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

QVariant ActiveItemWidget::activeItemData(int role = Qt::UserRole) const
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

void ActiveItemWidget::setItemData(int index, const QVariant &value, int role = Qt::UserRole)
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

    d->activeItem = d->items[current];
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

    d->activeItem = d->items[current];
    update();
}
