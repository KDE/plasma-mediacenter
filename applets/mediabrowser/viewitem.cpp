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
#include <QLinearGradient>
#include <QGraphicsSceneHoverEvent>

#include <KFileItem>
#include <KDirModel>
#include <kio/job.h>
#include <kio/previewjob.h>
#include <KUrl>

#include <Nepomuk/KRatingPainter>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>

#include <KDebug>

#include <Plasma/FrameSvg>
#include <Plasma/Theme>
#include <Plasma/PaintUtils>

static const int ITEM_VERTICAL_MARGIN = 15;

ViewItem::ViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent) : QGraphicsWidget(parent),
m_option(option),
m_index(QModelIndex()),
m_type(LocalFileItem),
m_frameSvg(new Plasma::FrameSvg(this)),
m_preview(0),
m_hoverRating(0),
m_rating(0),
m_resource(0),
m_nepomuk(false),
m_blurred(true)
{
    setContentsMargins(0, 0, 0, 0);

    m_frameSvg->setImagePath("widgets/viewitem");
    m_frameSvg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    m_frameSvg->setCacheAllRenderedFrames(true);
    m_frameSvg->setElementPrefix("hover");

    setAcceptedMouseButtons(0);
    
    m_nepomuk = Nepomuk::ResourceManager::instance()->initialized();
}

ViewItem::~ViewItem()
{}

void ViewItem::setStyleOption(const QStyleOptionViewItemV4 &option)
{
    m_option = option;
    askForFilePreview();
}

QStyleOptionViewItemV4 ViewItem::styleOption() const
{
    return m_option;
}

void ViewItem::setModelIndex(const QModelIndex &index)
{
    m_index = index;
    askForFilePreview();

    KFileItem item = m_index.data(KDirModel::FileItemRole).value<KFileItem>();
    if (item.isNull()) {
        return;
    }

    m_resource = new Nepomuk::Resource(item.url());
}

QModelIndex ViewItem::index() const
{
    return m_index;
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if (m_option.state & QStyle::State_MouseOver) {
        if (m_frameSvg->size() != option->rect.size()) {
            m_frameSvg->resizeFrame(option->rect.size());
        }
        m_frameSvg->paintFrame(painter, option->rect.topLeft());
        if (m_nepomuk) {
            if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
                KRatingPainter::paintRating(painter, ratingRect(option->rect), Qt::AlignLeft | Qt::AlignVCenter, m_rating, m_hoverRating);
            }
        }
    }

    if (!m_index.isValid()) {
        return;
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

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.setSize(QSize(option->rect.width() - option->rect.height(), option->rect.height()));
        textRect.moveTo(option->rect.height(), 0);

    } else if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        textRect.setSize(textRectSize());
        const int x = (option->rect.width() - decorationWidth) / 2;
        const int y = (option->rect.height() - decorationHeight - reflectionHeight - textRect.size().height()) / 2;
        decorationRect.moveTo(x, y);

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.moveTo(0, reflectionRect.bottom());
    }

    if (m_preview) {
        painter->drawPixmap(decorationRect.topLeft(), *m_preview);

        reflectionRect.moveTo(decorationRect.left(), decorationRect.top() + m_preview->height() + 1);
        drawReflection(painter, reflectionRect, *m_preview);
    } else {
        QVariant decoration = m_index.data(Qt::DecorationRole);
        if (decoration.type() == QVariant::Icon) {
            decoration.value<QIcon>().paint(painter, decorationRect);

            drawReflection(painter, reflectionRect, decoration.value<QIcon>());

        }
        // TODO: QPixmap possible code
    }

    painter->setFont(m_option.font);
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    QString text = m_index.data().toString();
    if (m_option.fontMetrics.width(text) > textRect.width()) {
        text = m_option.fontMetrics.elidedText(text, Qt::ElideMiddle, textRect.width());
    }

    if (m_blurred) {
        QPixmap blurredText = Plasma::PaintUtils::shadowText(text, m_option.font);
        QPointF txtPoint;
        if (m_option.displayAlignment & Qt::AlignLeft) {
            txtPoint.setX(textRect.x());
        }
        if (m_option.displayAlignment & Qt::AlignHCenter) {
            txtPoint.setX((textRect.width() - blurredText.width()) / 2);
        }
        if (m_option.displayAlignment & Qt::AlignVCenter) {
            txtPoint.setY((textRect.height() - blurredText.height()) / 2);
        }
        if (m_option.displayAlignment & Qt::AlignBottom) {
            txtPoint.setY(option->rect.height() - blurredText.height());
        }

        painter->drawPixmap(txtPoint, blurredText);
    } else {
        painter->drawText(textRect, m_option.displayAlignment, text);
    }
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QIcon &icon)
{
    const int decorationWidth = m_option.decorationSize.width();
    const int decorationHeight = decorationWidth;

    QPixmap pixmap(reflectionRect.width(), reflectionRect.height());
    pixmap.fill(Qt::transparent);

    QLinearGradient grad(reflectionRect.width() / 2, 0, reflectionRect.width() / 2, reflectionRect.height());
    grad.setColorAt(0, Qt::black);
    grad.setColorAt(1, Qt::transparent);

    QPainter p(&pixmap);
    p.fillRect(0, 0, reflectionRect.width(), reflectionRect.height(), grad);
    p.scale(1, -1);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    icon.paint(&p, 0, -decorationHeight, decorationWidth, decorationHeight);
    p.end();
    painter->drawPixmap(reflectionRect, pixmap);
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QPixmap &pm)
{
    const int decorationWidth = m_option.decorationSize.width();
    const int decorationHeight = decorationWidth;

    QPixmap pixmap(reflectionRect.width(), reflectionRect.height());
    pixmap.fill(Qt::transparent);

    QLinearGradient grad(reflectionRect.width() / 2, 0, reflectionRect.width() / 2, reflectionRect.height());
    grad.setColorAt(0, Qt::black);
    grad.setColorAt(1, Qt::transparent);

    QPainter p(&pixmap);
    p.fillRect(0, 0, reflectionRect.width(), reflectionRect.height(), grad);
    p.scale(1, -1);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    p.drawPixmap(0, -decorationHeight, decorationWidth, decorationHeight, pm);

    p.end();
    painter->drawPixmap(reflectionRect, pixmap);
}

void ViewItem::askForFilePreview()
{
    delete m_preview;
    m_preview = 0;
    // if the model is a KDirModel ask for previews
    if (!m_index.data(KDirModel::FileItemRole).isNull()) {
        KFileItem item = m_index.data(KDirModel::FileItemRole).value<KFileItem>();
        if (!item.mimetype().startsWith("image/")) {
            return;
        }
        KIO::PreviewJob *previewJob = KIO::filePreview(item.url(), m_option.decorationSize.width(), 0, 0, 0, false, true, 0);
        connect (previewJob, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)), this, SLOT(slotGotPreview(const KFileItem &, const QPixmap&)));
    }
}

void ViewItem::slotGotPreview(const KFileItem &item, const QPixmap &preview)
{
    Q_UNUSED(item)
    m_preview = new QPixmap(preview);
    update();
}

QSize ViewItem::itemSizeHint() const
{
    if (!m_index.isValid()) {
        return QSize(0, 0);
    }

    if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
        int height = qMax(m_option.decorationSize.width() + 2*ITEM_VERTICAL_MARGIN, m_option.fontMetrics.height());
        int width = m_option.decorationSize.width() + m_option.fontMetrics.width(m_index.data().toString());

        return QSize(width, height);
    } else if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        int width = m_option.decorationSize.width() * 2;
        int height = m_option.decorationSize.width() * 1.7;
        QRect textRect(0, 0, width, width - height);
        QRect bounding = m_option.fontMetrics.boundingRect(textRect, m_option.decorationAlignment, m_index.data().toString());

        height += bounding.height();
        return QSize(width, height);
    }

    return QSize(0, 0);

}

QSize ViewItem::textRectSize() const
{
    if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        int width = contentsRect().width();
        int height = m_option.decorationSize.width() * 1.7;
        QRect textRect(0, 0, width, width - height);
        QRect bounding = m_option.fontMetrics.boundingRect(textRect, m_option.decorationAlignment, m_index.data().toString());
        bounding.setWidth(width);

        return bounding.size();
    }

    return QSize();
}

QRect ViewItem::ratingRect(const QRect &contentsRect) const
{
    QRect ratingRect(0, 0, 72, 16);
    ratingRect.moveTo(contentsRect.right() - ratingRect.width(), contentsRect.y() + 1);

    return ratingRect;
}

void ViewItem::updateHoverRating(const QPoint &pos)
{
    if (!m_nepomuk) {
        return;
    }

    m_hoverRating = KRatingPainter::getRatingFromPosition(ratingRect(contentsRect().toRect()), Qt::AlignLeft | Qt::AlignVCenter, Qt::LeftToRight, pos);
    if (m_hoverRating == -1) {
        m_hoverRating = 0;
    }
    update();
}

void ViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_nepomuk) {
        event->ignore();
    }

    if (ratingRect(contentsRect().toRect()).contains(event->pos().toPoint())) {
        emit ratingActivated(KRatingPainter::getRatingFromPosition(ratingRect(contentsRect().toRect()),
                                                                   Qt::AlignLeft | Qt::AlignVCenter, Qt::LeftToRight, event->pos().toPoint()));
    } else {
        event->ignore();
    }
}

void ViewItem::setDrawBlurredText(bool set)
{
    if (set == m_blurred) {
        return;
    }

    m_blurred = set;
    update();
}

bool ViewItem::drawBlurredText()
{
    return m_blurred;
}

void ViewItem::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}
