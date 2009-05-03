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
#include "playlistdelegate.h"

// Qt
#include <QStyleOptionViewItem>
#include <QPainter>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QFontMetrics>

// KDE
#include <KIconLoader>
#include <KApplication>
#include <KDebug>

// Plasma
#include <Plasma/Theme>

static const int COVER_PIXMAP_SIZE = 64;
static const int ITEM_MARGIN = 2;
static const int REMOVE_BUTTON_SIZE = 16;

PlaylistDelegate::PlaylistDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

PlaylistDelegate::~PlaylistDelegate()
{
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->drawRect(option.rect);

    // here we apply the margin to the contents rect
    QRect contentsRect = option.rect;
    contentsRect.setWidth(contentsRect.width() - 2*ITEM_MARGIN);
    contentsRect.setHeight(contentsRect.height() - 2*ITEM_MARGIN);
    contentsRect.translate(ITEM_MARGIN, ITEM_MARGIN);
    painter->fillRect(contentsRect, Qt::green);

    // cover drawing
    QPixmap cover = index.data(CoverRole).isNull() ? KIconLoader::global()->loadIcon("audio-x-generic", KIconLoader::Desktop, COVER_PIXMAP_SIZE)
                    : index.data(CoverRole).value<QPixmap>().scaled(COVER_PIXMAP_SIZE, COVER_PIXMAP_SIZE);
    painter->drawPixmap(contentsRect.x(), contentsRect.y(), cover);

    // song title
    painter->drawText(ITEM_MARGIN + COVER_PIXMAP_SIZE + ITEM_MARGIN, contentsRect.y(), index.data(TrackNameRole).toString());
}

bool PlaylistDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QSize PlaylistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int width = 0;
    int height = 0;

    width += ITEM_MARGIN + COVER_PIXMAP_SIZE + ITEM_MARGIN;
    QFontMetrics fm = kapp->fontMetrics();
    width += fm.width(index.data(TrackNameRole).toString());

    height += ITEM_MARGIN + COVER_PIXMAP_SIZE + ITEM_MARGIN;

    return QSize(width, height);
}
