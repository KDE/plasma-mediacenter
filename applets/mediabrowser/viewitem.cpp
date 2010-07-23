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
#include <kio/job.h>
#include <kio/previewjob.h>
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

static const int ITEM_VERTICAL_MARGIN = 15;
static const float s_itemSpacing = 0.1;   //Factor of the size to be left as the margin/spacing

ViewItem::ViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent) : QGraphicsWidget(parent),
m_hoverRating(0),
m_rating(0),
m_resource(0),
m_blurred(true),
m_selectByIcon(true),
m_isSelected(false),
m_isNotFile(false)
{
    m_option = option;
    m_decorationSize = m_option.decorationSize.width();

    setContentsMargins(0, 0, 0, 0);

    m_frameSvg = new Plasma::FrameSvg(this);
    m_frameSvg->setImagePath("widgets/viewitem");
    m_frameSvg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
    m_frameSvg->setCacheAllRenderedFrames(true);
    m_frameSvg->setElementPrefix("hover");

    setAcceptedMouseButtons(0);

    m_nepomuk = true;
    if (!m_selectByIcon) {
        Nepomuk::ResourceManager::instance()->initialized();
    }

}

ViewItem::ViewItem(QGraphicsItem *parent) : QGraphicsWidget(parent),
    m_hoverRating(0),
    m_rating(0),
    m_resource(0),
    m_blurred(true),
    m_selectByIcon(true),
    m_isSelected(false),
    m_isNotFile(false)
{
    ViewItem::ViewItem(QStyleOptionViewItemV4(), parent);

    // this ctor has no way to pass the QStyleOptionViewItemV4
    // we create one with some convenient default values
    m_option.palette.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    m_option.font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DesktopFont);
    m_option.fontMetrics = QFontMetrics(m_option.font);
    m_option.decorationSize = QSize(KIconLoader::SizeEnormous, KIconLoader::SizeEnormous);
    m_option.locale = QLocale::system();
    m_option.widget = 0;
    m_option.state |= QStyle::State_Enabled;
    m_option.decorationPosition = QStyleOptionViewItem::Top;
    m_option.displayAlignment = Qt::AlignHCenter | Qt::AlignBottom;

    m_decorationSize = m_option.decorationSize.width();
}

ViewItem::~ViewItem()
{
    delete m_resource;
}

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
    if (m_index == index) {
        return;
    }

    m_blurredText = QPixmap();
    m_index = index;

    m_text = m_index.data().toString();
    m_icon = m_index.data(Qt::DecorationRole).value<QIcon>();

    KFileItem item = m_index.data(KDirModel::FileItemRole).value<KFileItem>();
    if (item.isNull()) {
        // nepomuk should be disabled for non-kfileitemmodel's (at least for now..)
        m_nepomuk = false;
        return;
    }

    bool ok;
    KUrl url = item.mostLocalUrl(ok);

    if (!ok) {
        // FIXME disabling nepomuk for non local files for now.
        m_nepomuk = false;
        return;
    }
    
    askForFilePreview();
    m_resource = new Nepomuk::Resource(url);
}

QModelIndex ViewItem::index() const
{
    return m_index;
}

void ViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    if (m_option.state & QStyle::State_MouseOver || m_isSelected == true) {
        if (m_frameSvg->size() != option->rect.size()) {
            m_frameSvg->resizeFrame(option->rect.size());
        }
        m_frameSvg->paintFrame(painter, option->rect.topLeft());
        if (m_nepomuk) {
            if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
                //KRatingPainter::paintRating(painter, ratingRect(option->rect), Qt::AlignLeft | Qt::AlignVCenter, m_rating, m_hoverRating);
            }
        }
        if (m_selectByIcon && m_isNotFile == false) {
            drawSelectIcon(painter, option);
        }

    }

    QRect textRect;
    QRect decorationRect;
    QRect reflectionRect;

    const int decorationSize = m_decorationSize;
    const int reflectionHeight = decorationSize * 0.33;

    decorationRect.setSize(QSize(decorationSize, decorationSize));
    reflectionRect.setSize(QSize(decorationSize, reflectionHeight));

    if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
        const int x = (option->rect.height() - decorationSize) / 2;
        const int y = (option->rect.height() - decorationSize - reflectionHeight) / 2;
        decorationRect.moveTo(x, y);

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.setSize(QSize(option->rect.width() - option->rect.height(), option->rect.height()));
        textRect.moveTo(option->rect.height(), 0);

    } else if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        textRect.setSize(textRectSize());

        const int x = (option->rect.width() - decorationSize) / 2;
        const int y = (option->rect.height() - decorationSize - reflectionHeight - textRect.height());
        decorationRect.moveTo(x, y);

        reflectionRect.moveTo(decorationRect.bottomLeft());

        textRect.moveTo(reflectionRect.bottomLeft());
    }

    if (!m_preview.isNull()) {
        QRect previewRect;
        previewRect.setSize(m_preview.size());
        previewRect.moveCenter(decorationRect.center());
        painter->drawPixmap(previewRect.topLeft(), m_preview);
        reflectionRect.moveTo(previewRect.left(), previewRect.bottom());
        reflectionRect.setWidth(previewRect.width());
        drawReflection(painter, reflectionRect, m_preview);

        if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
                textRect.moveTo(decorationRect.right(), (decorationRect.top()+reflectionRect.bottom())/2);
        } else if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
                textRect.moveTo(reflectionRect.bottomLeft());
        }
    } else {
        QIcon icon = m_icon;
        if (!icon.isNull()) {
            icon.paint(painter, decorationRect);
            drawReflection(painter, reflectionRect, icon);
        } else {
            kWarning() << "invalid icon";
        }
        // TODO: QPixmap possible code
    }

    painter->setFont(m_option.font);
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    QString text = m_text;
    if (m_option.fontMetrics.width(text) > textRect.width()) {
        text = m_option.fontMetrics.elidedText(text, Qt::ElideMiddle, textRect.width());
    }

    if (m_blurred) {
        if (m_blurredText.isNull()) {
            QColor textColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);
            QColor shadowColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::BackgroundColor);
            if (textColor.black() > 210) {
                textColor = shadowColor;
                shadowColor = Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor);
            }

            m_blurredText = Plasma::PaintUtils::shadowText(text, m_option.font, textColor, shadowColor);
        }

        QPointF txtPoint;
        if (m_option.displayAlignment & Qt::AlignLeft) {
            txtPoint.setX(textRect.x());
        }
        if (m_option.displayAlignment & Qt::AlignHCenter) {
            txtPoint.setX((textRect.width() - m_blurredText.width()) / 2);
        }
        if (m_option.displayAlignment & Qt::AlignVCenter) {
            txtPoint.setY((textRect.height() - m_blurredText.height()) / 2);
        }
        if (m_option.displayAlignment & Qt::AlignBottom) {
            txtPoint.setY(option->rect.height() - m_blurredText.height());
        }

        painter->drawPixmap(txtPoint, m_blurredText);
    } else {
        painter->drawText(textRect, m_option.displayAlignment, text);
    }
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QIcon &icon)
{
    drawReflection(painter, reflectionRect, icon.pixmap(m_option.decorationSize));
}

void ViewItem::drawReflection(QPainter *painter, const QRect &reflectionRect, const QPixmap &pm)
{
    if (!m_reflection.isNull()) {
        painter->drawPixmap(reflectionRect, m_reflection);
        return;
    }

    const int decorationWidth = m_option.decorationSize.width();
    const int decorationHeight = decorationWidth;

    m_reflection = QPixmap(reflectionRect.width(), reflectionRect.height());
    kDebug() << "reflection size" << m_reflection.size();
    m_reflection.fill(Qt::transparent);

    QLinearGradient grad(reflectionRect.width() / 2, 0, reflectionRect.width() / 2, reflectionRect.height());
    grad.setColorAt(0, Qt::black);
    grad.setColorAt(1, Qt::transparent);

    QPainter p(&m_reflection);
    p.fillRect(0, 0, reflectionRect.width(), reflectionRect.height(), grad);
    p.scale(1, -1);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);

    p.setOpacity(0.7);
    p.drawPixmap(QRect(0, -reflectionRect.height(), reflectionRect.width(), reflectionRect.height()),
                 pm,
                 QRect(0, pm.height() - reflectionRect.height(), reflectionRect.width(), reflectionRect.height()));
    p.end();

    // here we add the blur to the reflection
    QImage img = m_reflection.toImage();
    expblur<5,5>(img, 5);
    m_reflection = QPixmap::fromImage(img);

    painter->drawPixmap(reflectionRect, m_reflection);
}

void ViewItem::askForFilePreview()
{
    m_preview = QPixmap();
    // if the model is a KDirModel ask for previews
    if (!m_index.data(KDirModel::FileItemRole).isNull()) {
        KFileItem item = m_index.data(KDirModel::FileItemRole).value<KFileItem>();

        bool ok;
        KUrl url = item.mostLocalUrl(ok);
        if (!ok) {
            return;
        }
        KIO::PreviewJob *previewJob = KIO::filePreview(url, m_option.decorationSize.width(), 0, 0, 0, false, true, 0);
        connect(previewJob, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)), this, SLOT(slotGotPreview(const KFileItem &, const QPixmap&)));
    }
}

void ViewItem::slotGotPreview(const KFileItem &item, const QPixmap &preview)
{
    m_preview = preview;
    // we need to invalidate the pixmap when the preview gets loaded
    m_reflection = QPixmap();
    update();
}

QSize ViewItem::itemSizeHint() const
{
    if (!m_index.isValid()) {
        return QSize(0, 0);
    }

    if (m_option.decorationPosition == QStyleOptionViewItem::Left) {
        int decorationHeight = m_option.decorationSize.height() * (1 + 2*s_itemSpacing);
        int height = qMax(decorationHeight, m_option.fontMetrics.height());
        int width = m_option.decorationSize.width() + m_option.fontMetrics.width(m_text);

        return QSize(width, height);
    }

    if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        int width = m_option.decorationSize.width();
        int height = m_option.decorationSize.width();
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
    if (m_option.decorationPosition == QStyleOptionViewItem::Top) {
        const int width = contentsRect().width();
        const int height = m_option.decorationSize.width() * 1.7;
        QRect textRect(0, 0, width, width - height);
        QRect bounding = m_option.fontMetrics.boundingRect(textRect, m_option.decorationAlignment, m_text);
        bounding.setWidth(width);

        return bounding.size();
    }

    return QSize();
}

QRect ViewItem::ratingRect(const QRect &contentsRect) const
{
    QRect ratingRect(0, 0, 0, 0);

    return ratingRect;
}

QRect ViewItem::selectRect(const QRect &contentsRect) const
{
    if (!m_selectByIcon || m_isNotFile) { //TODO: Do the inactivating of selection sign differently
        return QRect(0,0,0,0);
    }

    QRect selectRect(0, 0, 32, 32);
    selectRect.moveTo(contentsRect.right() - selectRect.width() - 5, contentsRect.bottom() - selectRect.height() - 5);

    return selectRect;
}

void ViewItem::updateHoverRating(const QPoint &pos)
{
    if (!m_nepomuk) {
        return;
    }

    //m_hoverRating = KRatingPainter::getRatingFromPosition(ratingRect(contentsRect().toRect()),
    //Qt::AlignLeft | Qt::AlignVCenter, Qt::LeftToRight, pos);
    if (m_hoverRating == -1) {
        m_hoverRating = 0;
    }
    update();
}

void ViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_nepomuk) {
        event->ignore();
        return;
    }

    if (ratingRect(contentsRect().toRect()).contains(event->pos().toPoint())) {
        //emit ratingActivated(KRatingPainter::getRatingFromPosition(ratingRect(contentsRect().toRect()),
        //                                                           Qt::AlignLeft | Qt::AlignVCenter, Qt::LeftToRight, event->pos().toPoint()));
    }
    if (selectRect(contentsRect().toRect()).contains(event->pos().toPoint())) {
            emit itemSelected();
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

void ViewItem::drawSelectIcon(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
    const char* icon = m_isSelected ? "list-remove" : "list-add";
    m_selectIcon = KIconLoader::global()->loadIcon(icon, KIconLoader::NoGroup, qMin(16,16));
    painter->drawPixmap(selectRect(option->rect), m_selectIcon);
}

bool ViewItem::isSelected()
{
    return m_isSelected;
}

void ViewItem::setSelected(bool set)
{
    m_isSelected = set;
}

void ViewItem::showCornerIcons(bool set)
{
    //TODO: Find a way to turn off the actions in the item corners for touch based devices
}

void ViewItem::setIsNotFile(bool set)
{
    m_isNotFile = set;
}

void ViewItem::setIcon(const QIcon &icon)
{
    m_icon = icon;
}

QIcon ViewItem::icon() const
{
    return m_icon;
}

void ViewItem::setText(const QString &text)
{
    m_text = text;
}

QString ViewItem::text() const
{
    return m_text;
}

void ViewItem::setDecorationSize(int size)
{
    m_decorationSize = size;
}

int ViewItem::decorationSize() const
{
    return m_decorationSize;
}
