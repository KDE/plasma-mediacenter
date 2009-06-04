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
#include <QVariant>
#include <QIcon>
#include <QPixmap>

#include <KFileItemDelegate>

#include <Plasma/FrameSvg>

ViewItem::ViewItem(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_type(LocalFileItem),
m_frameSvg(new Plasma::FrameSvg(this))
{
    setContentsMargins(0, 0, 0, 0);

    m_frameSvg->setImagePath("widgets/viewitem");
    m_frameSvg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    m_frameSvg->setCacheAllRenderedFrames(true);
    m_frameSvg->setElementPrefix("hover");
}

ViewItem::~ViewItem()
{}

void ViewItem::setStyleOption(const QStyleOptionViewItemV4 &option)
{
    m_option = option;
}

QStyleOptionViewItemV4 ViewItem::styleOption() const
{
    return m_option;
}

void ViewItem::setModelIndex(const QModelIndex &index)
{
    m_index = index;
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if (m_option.state & QStyle::State_MouseOver) {
        if (m_frameSvg->size() != option->rect.size()) {
            m_frameSvg->resizeFrame(option->rect.size());
        }
        m_frameSvg->paintFrame(painter, option->rect.topLeft());
    }

    QRect textRect;
    QRect decorationRect;
    QRect reflectionRect;

    const int decorationWidth = m_option.decorationSize.width();
    const int decorationHeight = decorationWidth;
    const int reflectionHeight = decorationHeight * 0.33;

    decorationRect.setSize(QSize(decorationWidth, decorationHeight));
    reflectionRect.setSize(QSize(decorationWidth, reflectionHeight));

    if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
        const int x = (option->rect.height() - decorationHeight) / 2;
        const int y = (option->rect.height() - decorationHeight - reflectionHeight) / 2;
        decorationRect.moveTo(x, y);

        textRect.setSize(QSize(option->rect.width() - option->rect.height(), option->rect.height()));
        textRect.moveTo(option->rect.height(), 0);
    } else if ( m_option.decorationPosition == QStyleOptionViewItem::Top) {
        // TODO: fill in code for the Top mode :)
    }

    QVariant decoration = m_index.data(Qt::DecorationRole);
    if (decoration.type() == QVariant::Icon) {
        decoration.value<QIcon>().paint(painter, decorationRect);
    }
    // TODO: QPixmap possible code

    painter->setFont(m_option.font);
    painter->setPen(m_option.palette.color(QPalette::Text));
    painter->drawText(textRect, m_option.displayAlignment, m_index.data().toString());
}
