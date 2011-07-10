/*
 *   Copyright 2009 Marco Martin <notmart@gmail.com>
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
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

#include "mediatoolbox.h"

#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>

#include <KIconLoader>

#include <Plasma/Animator>
#include <Plasma/Containment>
#include <Plasma/IconWidget>
#include <Plasma/ItemBackground>
#include <Plasma/PaintUtils>
#include <Plasma/FrameSvg>
#include <Plasma/Svg>


MediaToolBox::MediaToolBox(Plasma::Containment *parent)
   : Plasma::AbstractToolBox(parent),
     m_containment(parent),
     m_icon("plasma"),
     m_iconSize(KIconLoader::SizeSmall, KIconLoader::SizeSmall),
     m_animHighlightId(0),
     m_animHighlightFrame(0),
     m_hovering(false),
     m_showing(false),
     m_location(Plasma::BottomEdge)
{
    setZValue(9000);
    resize(KIconLoader::SizeMedium, KIconLoader::SizeMedium);
    setAcceptHoverEvents(true);

    m_background = new Plasma::Svg(this);
    m_background->setImagePath("widgets/toolbox");
    m_background->setContainsMultipleImages(true);
    setLocation(Plasma::BottomEdge);

    connect(m_containment, SIGNAL(geometryChanged()), this, SLOT(containmentGeometryChanged()));
    containmentGeometryChanged();

    connect(Plasma::Animator::self(), SIGNAL(movementFinished(QGraphicsItem*)),
            this, SLOT(movementFinished(QGraphicsItem*)));
}

MediaToolBox::~MediaToolBox()
{
}

bool MediaToolBox::isShowing() const
{
    return m_showing;
}

void MediaToolBox::setShowing(const bool show)
{
    m_showing = show;

    emit toggled();
    emit visibilityChanged(show);
}


void MediaToolBox::addTool(QAction *action)
{
    Plasma::IconWidget *button = new Plasma::IconWidget(this);
    button->setOrientation(Qt::Horizontal);
    button->setTextBackgroundColor(QColor());
    button->setAction(action);

    if (m_location == Plasma::LeftEdge || m_location == Plasma::RightEdge) {
        button->setOrientation(Qt::Vertical);
    } else {
        button->setOrientation(Qt::Horizontal);
    }

    m_actionButtons[action] = button;
}

void MediaToolBox::removeTool(QAction *action)
{
    if (m_actionButtons.contains(action)) {
        Plasma::IconWidget *button = m_actionButtons.value(action);
        m_actionButtons.remove(action);
        button->deleteLater();
    }
}

void MediaToolBox::setLocation(Plasma::Location location)
{
    m_location = location;
    if (location == Plasma::LeftEdge || location == Plasma::RightEdge) {
        foreach (Plasma::IconWidget *icon, m_actionButtons) {
            icon->setOrientation(Qt::Vertical);
        }
    } else {
        foreach (Plasma::IconWidget *icon, m_actionButtons) {
            icon->setOrientation(Qt::Horizontal);
        }
    }
    containmentGeometryChanged();
}

Plasma::Location MediaToolBox::location() const
{
    return m_location;
}

void MediaToolBox::containmentGeometryChanged()
{
    switch (m_location) {
    case Plasma::TopEdge:
        setPos(0, 0);
        break;
    case Plasma::BottomEdge:
        setPos(0, m_containment->size().height()-size().height());
        break;
    case Plasma::LeftEdge:
        setPos(0, 0);
        break;
    case Plasma::RightEdge:
        setPos(m_containment->size().width()-size().width(), 0);
        break;
    default:
        ;
        break;
    }
}

void MediaToolBox::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void MediaToolBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setShowing(!isShowing());
}

void MediaToolBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    QPoint iconPos;
    QString svgElement;

    switch (m_location) {
    case Plasma::TopEdge:
    case Plasma::LeftEdge:
        iconPos = QPoint(2, 2);
        svgElement = "desktop-northwest";
        break;
    case Plasma::RightEdge:
        iconPos = QPoint(size().width() - m_iconSize.width() - 2, 2);
        svgElement = "desktop-northeast";
        break;
    case Plasma::BottomEdge:
    default:
        iconPos = QPoint(2, size().height() - m_iconSize.height() - 2);
        svgElement = "desktop-southwest";
        break;
    }

    m_background->paint(painter, boundingRect(), svgElement);

    if (qFuzzyCompare(qreal(1.0), m_animHighlightFrame)) {
        m_icon.paint(painter, QRect(iconPos, m_iconSize));
    } else if (qFuzzyCompare(qreal(1.0), 1 + m_animHighlightFrame)) {
        m_icon.paint(painter, QRect(iconPos, m_iconSize),
                      Qt::AlignCenter, QIcon::Disabled, QIcon::Off);
    } else {
        QPixmap disabled = m_icon.pixmap(m_iconSize, QIcon::Disabled, QIcon::Off);
        QPixmap enabled = m_icon.pixmap(m_iconSize);
        QPixmap result = Plasma::PaintUtils::transition(
            m_icon.pixmap(m_iconSize, QIcon::Disabled, QIcon::Off),
            m_icon.pixmap(m_iconSize), m_animHighlightFrame);
        painter->drawPixmap(QRect(iconPos, m_iconSize), result);
    }
}

void MediaToolBox::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (isShowing() || m_hovering) {
        QGraphicsWidget::hoverEnterEvent(event);
        return;
    }
    Plasma::Animator *animdriver = Plasma::Animator::self();
    if (m_animHighlightId) {
        animdriver->stopCustomAnimation(m_animHighlightId);
    }
    m_hovering = true;
    m_animHighlightId =
        animdriver->customAnimation(
            10, 240, Plasma::Animator::EaseInCurve, this, "animateHighlight");

    QGraphicsWidget::hoverEnterEvent(event);
}

void MediaToolBox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //kDebug() << event->pos() << event->scenePos()
    //         << d->toolBacker->rect().contains(event->scenePos().toPoint());
    if (!m_hovering || isShowing()) {
        QGraphicsWidget::hoverLeaveEvent(event);
        return;
    }

    Plasma::Animator *animdriver = Plasma::Animator::self();
    if (m_animHighlightId) {
        animdriver->stopCustomAnimation(m_animHighlightId);
    }
    m_hovering = false;
    m_animHighlightId =
        animdriver->customAnimation(
            10, 240, Plasma::Animator::EaseOutCurve, this, "animateHighlight");

    QGraphicsWidget::hoverLeaveEvent(event);
}

void MediaToolBox::animateHighlight(qreal progress)
{
    if (m_hovering) {
        m_animHighlightFrame = progress;
    } else {
        m_animHighlightFrame = 1.0 - progress;
    }

    if (progress >= 1) {
        m_animHighlightId = 0;
    }

    update();
}

#include "mediatoolbox.moc"
