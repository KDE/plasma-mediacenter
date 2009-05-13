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
#include "utils/utils.h"

// Qt
#include <QMouseEvent>
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
#include <Plasma/PaintUtils>
#include <Plasma/FrameSvg>

static const int COVER_SIZE = 64;
static const int COVER_SMALL_SIZE = 48;
static const int ITEM_MARGIN = 2;
static const int TOOL_BUTTON_SIZE = 16;
static const int SPACING = 5;

PlaylistDelegate::PlaylistDelegate(QObject *parent) : QStyledItemDelegate(parent), m_frameSvg(new Plasma::FrameSvg(this))
{
    m_frameSvg->setImagePath("widgets/viewitem");
    m_frameSvg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    m_frameSvg->setCacheAllRenderedFrames(true);
    m_frameSvg->setElementPrefix("hover");
}

PlaylistDelegate::~PlaylistDelegate()
{
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    // here we apply the margin to the contents rect
    QRect contentsRect = option.rect;
    contentsRect.setWidth(contentsRect.width() - 2*ITEM_MARGIN);
    contentsRect.setHeight(contentsRect.height() - 2*ITEM_MARGIN);
    contentsRect.translate(ITEM_MARGIN, ITEM_MARGIN);

    // hover state
    if (option.state & QStyle::State_MouseOver) {
        painter->save();
        m_frameSvg->resizeFrame(option.rect.size());
        m_frameSvg->paintFrame(painter, option.rect.topLeft());
        painter->restore();
        MediaCenter::drawCloseEmblem(painter, removeButtonRect(contentsRect));
        MediaCenter::drawReloadEmblem(painter, reloadButtonRect(contentsRect));
    }

    // cover drawing
    int size = option.state & QStyle::State_MouseOver ? COVER_SIZE : COVER_SMALL_SIZE;
    QPixmap cover = index.data(CoverRole).isNull() ? KIconLoader::global()->loadIcon("x-media-podcast", KIconLoader::Desktop, size, KIconLoader::DisabledState)
                    : index.data(CoverRole).value<QPixmap>().scaled(size, size);

    if (option.state & QStyle::State_MouseOver) {
        painter->drawPixmap(contentsRect.x(), contentsRect.y(), cover);
    } else {
        painter->drawPixmap(contentsRect.x() + (COVER_SIZE - COVER_SMALL_SIZE)/2, contentsRect.y() + (COVER_SIZE - COVER_SMALL_SIZE)/2, cover);
    }

    painter->setBrush(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));

    // the y for the text
    int y = contentsRect.y();
    // the x for the text
    int x = contentsRect.x() + COVER_SIZE + ITEM_MARGIN;

    // song title
    if (option.state & QStyle::State_MouseOver &&
        qGray(Plasma::Theme::defaultTheme()->color(Plasma::Theme::HighlightColor).rgb()) > 192) {
        QPixmap title = Plasma::PaintUtils::shadowText(index.data(TrackNameRole).toString(),
                                                       Plasma::Theme::defaultTheme()->color(Plasma::Theme::HighlightColor),
                                                       Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
        painter->drawPixmap(x - 2, y + 1, title);
    } else {
        painter->drawText(x, y + option.fontMetrics.height(), index.data(TrackNameRole).toString());
    }

    y += option.fontMetrics.height() + ITEM_MARGIN;

    // artist
    painter->save();
    painter->setPen(option.state & QStyle::State_MouseOver ?
                    Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor) :
                                                         Plasma::Theme::defaultTheme()->color(Plasma::Theme::HighlightColor));
    painter->drawText(x, y + option.fontMetrics.height(), index.data(ArtistRole).toString());
    painter->restore();


}

bool PlaylistDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonPress) {
        if (removeButtonRect(getContentsRect(option.rect)).contains(static_cast<QMouseEvent*>(event)->pos())) {
            emit removeRequested(index);
        } else if (reloadButtonRect(getContentsRect(option.rect)).contains(static_cast<QMouseEvent*>(event)->pos())) {
            emit reloadRequested(index);
        }
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

QRect PlaylistDelegate::getContentsRect(const QRect &rect) const
{
    QRect contentsRect = rect;
    contentsRect.setWidth(contentsRect.width() - 2*ITEM_MARGIN);
    contentsRect.setHeight(contentsRect.height() - 2*ITEM_MARGIN);
    contentsRect.translate(ITEM_MARGIN, ITEM_MARGIN);

    return contentsRect;
}

QRect PlaylistDelegate::removeButtonRect(const QRect &contentsRect) const
{
    return QRect(contentsRect.right() - TOOL_BUTTON_SIZE, contentsRect.y(), TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
}

QRect PlaylistDelegate::reloadButtonRect(const QRect &contentsRect) const
{
    return QRect(contentsRect.right() - TOOL_BUTTON_SIZE, contentsRect.y() + TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE, TOOL_BUTTON_SIZE);
}

QSize PlaylistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int width = 0;
    int height = 0;

    width += ITEM_MARGIN + COVER_SIZE + ITEM_MARGIN;
    const QFontMetrics &fm = option.fontMetrics;
    width += fm.width(index.data().toString());
    width += SPACING + TOOL_BUTTON_SIZE;

    height += ITEM_MARGIN + COVER_SIZE + ITEM_MARGIN;

    kDebug() << "returning QSize(" << width << "," << height << ")" << "for" << index.data().toString();
    return QSize(width, height);
}

#include "playlistdelegate.moc"
