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
#include "blur.cpp"

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
#include <KUrl>
#include <KIconLoader>

#include <Nepomuk/KRatingPainter>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>
#include <nepomuk/kratingwidget.h>

#include <KDebug>

#include <Plasma/FrameSvg>
#include <Plasma/Theme>
#include <Plasma/PaintUtils>

using namespace MediaCenter;

static const int ITEM_VERTICAL_MARGIN = 15;
static const float s_itemSpacing = 0.1;   //Factor of the size to be left as the margin/spacing

class ViewItem::ViewItemPrivate
{
public:
    ViewItemPrivate(ViewItem *q) : q(q),
        frameSvg(0),
        blurred(true),
        selectByIcon(false),
        selected(false)
    {}

    ViewItem *q;
    QStyleOptionViewItemV4 option;
    QModelIndex index;
    Plasma::FrameSvg *frameSvg;
    QPixmap pixmap;
    QPixmap selectIcon;
    QPixmap blurredText;
    QPixmap reflection;
    bool blurred;
    bool selectByIcon;
    bool selected;
    QIcon icon;
    QString text;
    int decorationSize;
};

ViewItem::ViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent) : QGraphicsWidget(parent),
    d(new ViewItemPrivate(this))
{
    d->option = option;
    d->decorationSize = d->option.decorationSize.width();

    setContentsMargins(0, 0, 0, 0);

    d->frameSvg = new Plasma::FrameSvg(this);
    d->frameSvg->setImagePath("widgets/viewitem");
    d->frameSvg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    d->frameSvg->setCacheAllRenderedFrames(true);
    d->frameSvg->setElementPrefix("hover");

    setAcceptedMouseButtons(0);

    if (!d->selectByIcon) {
        Nepomuk::ResourceManager::instance()->initialized();
    }

}

ViewItem::ViewItem(QGraphicsItem *parent) : QGraphicsWidget(parent),
    d(new ViewItemPrivate(this))
{
    ViewItem::ViewItem(QStyleOptionViewItemV4(), parent);

    // this ctor has no way to pass the QStyleOptionViewItemV4
    // we create one with some convenient default values
    d->option.palette.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    d->option.font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DesktopFont);
    d->option.fontMetrics = QFontMetrics(d->option.font);
    d->option.decorationSize = QSize(KIconLoader::SizeEnormous, KIconLoader::SizeEnormous);
    d->option.locale = QLocale::system();
    d->option.widget = 0;
    d->option.state |= QStyle::State_Enabled;
    d->option.decorationPosition = QStyleOptionViewItem::Top;
    d->option.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;

    d->decorationSize = d->option.decorationSize.width();
}

ViewItem::~ViewItem()
{
    delete d;
}

void ViewItem::setStyleOption(const QStyleOptionViewItemV4 &option)
{
    d->option = option;
}

QStyleOptionViewItemV4 ViewItem::styleOption() const
{
    return d->option;
}

void ViewItem::setModelIndex(const QModelIndex &index)
{
    if (d->index == index) {
        return;
    }

    d->blurredText = QPixmap();
    d->index = index;

    d->text = d->index.data().toString();
    d->icon = d->index.data(Qt::DecorationRole).value<QIcon>();
}

QModelIndex ViewItem::modelIndex() const
{
    return d->index;
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if (d->option.state & QStyle::State_MouseOver || d->option.state & QStyle::State_Selected) {
        if (d->frameSvg->size() != option->rect.size()) {
            d->frameSvg->resizeFrame(option->rect.size());
        }
        d->frameSvg->paintFrame(painter, option->rect.topLeft());
    }

    QRect textRect;
    QRect decorationRect;
    QRect reflectionRect;

    const int decorationSize = d->decorationSize;
    const int reflectionHeight = decorationSize * 0.33;

    decorationRect.setSize(QSize(decorationSize, decorationSize));
    reflectionRect.setSize(QSize(decorationSize, reflectionHeight));

    if (d->option.decorationPosition == QStyleOptionViewItem::Left) {
        const int x = (option->rect.height() - decorationSize) / 2;
        const int y = (option->rect.height() - decorationSize - reflectionHeight) / 2;
        decorationRect.moveTo(x, y);

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.setSize(QSize(option->rect.width() - option->rect.height(), option->rect.height()));
        textRect.moveTo(option->rect.height(), 0);

    } else if (d->option.decorationPosition == QStyleOptionViewItem::Top) {
        textRect.setSize(textRectSize());

        const int x = (option->rect.width() - decorationSize) / 2;
        const int y = (option->rect.height() - decorationSize - reflectionHeight - textRect.height());
        decorationRect.moveTo(x, y);

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.moveTo(reflectionRect.bottomLeft());
    }

    if (!d->pixmap.isNull()) {
        QRect pixmapRect;
        pixmapRect.setSize(d->pixmap.size());
        pixmapRect.moveCenter(decorationRect.center());
        painter->drawPixmap(pixmapRect.topLeft(), d->pixmap);
        reflectionRect.moveTo(pixmapRect.left(), pixmapRect.bottom());
        reflectionRect.setWidth(pixmapRect.width());
        drawReflection(painter, reflectionRect, d->pixmap);

        if (d->option.decorationPosition == QStyleOptionViewItem::Left) {
                textRect.moveTo(decorationRect.right(), (decorationRect.top()+reflectionRect.bottom())/2);
        } else if (d->option.decorationPosition == QStyleOptionViewItem::Top) {
                textRect.moveTo(reflectionRect.bottomLeft());
        }
    } else {
        QIcon icon = d->icon;
        if (!icon.isNull()) {
            icon.paint(painter, decorationRect);
            drawReflection(painter, reflectionRect, icon);
        } else {
            kWarning() << "invalid icon";
        }
    }

    painter->setFont(d->option.font);
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    QString text = d->text;
    if (d->option.fontMetrics.width(text) > textRect.width()) {
        text = d->option.fontMetrics.elidedText(text, Qt::ElideMiddle, textRect.width());
    }

    if (d->blurred) {
        if (d->blurredText.isNull()) {
            QColor textColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);
            QColor shadowColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor);
            if (textColor.black() > 210) {
                textColor = shadowColor;
                shadowColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);
            }

            d->blurredText = Plasma::PaintUtils::shadowText(text, d->option.font, textColor, shadowColor);
        }

        QPointF txtPoint;
        if (d->option.displayAlignment & Qt::AlignLeft) {
            txtPoint.setX(textRect.x());
        }
        if (d->option.displayAlignment & Qt::AlignHCenter) {
            txtPoint.setX((textRect.width() - d->blurredText.width()) / 2);
        }
        if (d->option.displayAlignment & Qt::AlignVCenter) {
            txtPoint.setY((textRect.height() - d->blurredText.height()) / 2);
        }
        if (d->option.displayAlignment & Qt::AlignBottom) {
            txtPoint.setY(option->rect.height() - d->blurredText.height());
        }

        painter->drawPixmap(txtPoint, d->blurredText);
    } else {
        painter->drawText(textRect, d->option.displayAlignment, text);
    }
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QIcon &icon)
{
    drawReflection(painter, reflectionRect, icon.pixmap(d->option.decorationSize));
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QPixmap &pm)
{
    if (!d->reflection.isNull()) {
        painter->drawPixmap(reflectionRect, d->reflection);
        return;
    }

    const int decorationWidth = d->option.decorationSize.width();
    const int decorationHeight = decorationWidth;

    d->reflection = QPixmap(reflectionRect.width(), reflectionRect.height());
    kDebug() << "reflection size" << d->reflection.size();
    d->reflection.fill(Qt::transparent);

    QLinearGradient grad(reflectionRect.width() / 2, 0, reflectionRect.width() / 2, reflectionRect.height());
    grad.setColorAt(0, Qt::black);
    grad.setColorAt(1, Qt::transparent);

    QPainter p(&d->reflection);
    p.fillRect(0, 0, reflectionRect.width(), reflectionRect.height(), grad);
    p.scale(1, -1);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    p.setOpacity(0.7);
    p.drawPixmap(QRect(0, -reflectionRect.height(), reflectionRect.width(), reflectionRect.height()),
                 pm,
                 QRect(0, pm.height() - reflectionRect.height(), reflectionRect.width(), reflectionRect.height()));
    p.end();

    // here we add the blur to the reflection
    QImage img = d->reflection.toImage();
    expblur<5,5>(img, 5);
    d->reflection = QPixmap::fromImage(img);

    painter->drawPixmap(reflectionRect, d->reflection);
}

QSize ViewItem::itemSizeHint() const
{
    if (!d->index.isValid()) {
        return QSize(0, 0);
    }

    if (d->option.decorationPosition == QStyleOptionViewItem::Left) {
        int decorationHeight = d->option.decorationSize.height() * (1 + 2*s_itemSpacing);
        int height = qMax(decorationHeight, d->option.fontMetrics.height());
        int width = d->option.decorationSize.width() + d->option.fontMetrics.width(d->text);

        return QSize(width, height);
    }

    if (d->option.decorationPosition == QStyleOptionViewItem::Top) {
        int width = d->option.decorationSize.width();
        int height = d->option.decorationSize.width();
        int reflectionHeight = height * 0.33;
        height += reflectionHeight + textRectSize().height();

        height += s_itemSpacing*height;
        width += s_itemSpacing*width;
        return QSize(width, height);    //Spacing between items
    }

    return QSize(0, 0);

}

QSize ViewItem::textRectSize() const
{
    if (d->option.decorationPosition == QStyleOptionViewItem::Top) {
        const int width = contentsRect().width();
        const int height = d->option.decorationSize.width() * 1.7;
        QRect textRect(0, 0, width, width - height);
        QRect bounding = d->option.fontMetrics.boundingRect(textRect, d->option.decorationAlignment, d->text);
        bounding.setWidth(width);

        return bounding.size();
    }

    return QSize();
}

void ViewItem::setDrawBlurredText(bool set)
{
    if (set == d->blurred) {
        return;
    }

    d->blurred = set;
    update();
}

bool ViewItem::drawBlurredText()
{
    return d->blurred;
}

void ViewItem::keyPressEvent(QKeyEvent *event)
{
    event->ignore();
}

void ViewItem::setIcon(const QIcon &icon)
{
    d->icon = icon;
}

QIcon ViewItem::icon() const
{
    return d->icon;
}

void ViewItem::setText(const QString &text)
{
    d->text = text;
}

QString ViewItem::text() const
{
    return d->text;
}

void ViewItem::setDecorationSize(int size)
{
    d->decorationSize = size;
}

int ViewItem::decorationSize() const
{
    return d->decorationSize;
}

void ViewItem::setPixmap(const QPixmap &pixmap)
{
    d->pixmap = pixmap;
    d->reflection = QPixmap();
}

QPixmap ViewItem::pixmap() const
{
    return d->pixmap;
}
