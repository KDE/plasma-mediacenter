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

// Plasma
#include <Plasma/ScrollBar>

// KDE
#include <KIconLoader>

// Qt
#include <QScrollBar>

AbstractMediaItemView::AbstractMediaItemView(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_scrollBar(new Plasma::ScrollBar(this)),
m_model(0),
m_delegate(0)

{
    setIconSize(KIconLoader::global()->currentSize(KIconLoader::Desktop));
}

AbstractMediaItemView::~AbstractMediaItemView()
{}

void AbstractMediaItemView::setIconSize(int size)
{
    m_iconSize = size;
    update();
}

int AbstractItemView::iconSize() const
{
    return m_iconSize;
}

void AbstractMediaItemView::setModel(QAbstractItemModel *model)
{
    if (m_model) {
        delete m_model;
    }

    m_model = model;
}

QAbstractItemModel* AbstractMediaItemView::model()
{
    return m_model;
}

void AbstractMediaItemView::setItemDelegate(KFileItemDelegate *delegate)
{
    if (m_delegate) {
        delete m_delegate;
    }

    m_delegate = delegate;
}

KFileItemDelegate* AbstractMediaItemView::itemDelegate()
{
    return m_delegate;
}

QScrollBar* AbstractMediaItemView::verticalScrollBar()
{
    return m_scrollBar->nativeWidget();
}

QRect AbstractMediaItemView::contentsArea() const
{
    QRect contents = contentsRect().toRect();
    contents.setWidth(contents.width() - m_scrollBar->nativeWidget()->width());

    return contents;
}
