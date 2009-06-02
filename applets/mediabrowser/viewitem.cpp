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
#include "viewitem.h"

#include <QAbstractItemModel>
#include <QAbstractItemDelegate>
#include <QPainter>

ViewItem::ViewItem(QGraphicsItem *parent) : QGraphicsWidget(parent)
{
    setContentsMargins(0, 0, 0, 0);
}

ViewItem::~ViewItem()
{}

void ViewItem::setItemDelegate(QAbstractItemDelegate *delegate)
{
    m_delegate = delegate;
}

void ViewItem::setStyleOption(const QStyleOptionViewItemV4 &option)
{
    m_option = option;
}

void ViewItem::setModelIndex(const QModelIndex &index)
{
    m_index = index;
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    m_option.rect = option->rect;
    m_delegate->paint(painter, m_option, m_index);
}
