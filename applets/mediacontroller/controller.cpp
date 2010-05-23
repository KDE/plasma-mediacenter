/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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
#include "controller.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <KIcon>
#include <KDebug>

// Plasma
#include <Plasma/Theme>

MediaController::MediaController(QObject *parent, const QVariantList &args)
    : MediaCenter::PlaybackControl(parent, args),
    m_svg(new Plasma::FrameSvg(this)),
    m_layoutHorizontalTop(new QGraphicsLinearLayout(this)),
    m_layoutTopLeft(new QGraphicsLinearLayout(Qt::Horizontal)),
    m_layoutTopMiddle(new QGraphicsLinearLayout(Qt::Horizontal)),
    m_layoutTopRight(new QGraphicsLinearLayout(Qt::Horizontal))

{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

MediaController::~MediaController()
{}

void MediaController::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    setBackgroundHints(NoBackground);
    setMarginsFromTheme();
}

void MediaController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event)

    m_svg->resizeFrame(rect().size());
}

void MediaController::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    m_svg->paintFrame(painter, option->rect);
}

int MediaController::iconSizeFromCurrentSize() const
{
    const int size = contentsRect().height();
    return size;
}

void MediaController::init()
{
    m_svg->setEnabledBorders(Plasma::FrameSvg::BottomBorder);
    m_svg->setCacheAllRenderedFrames(true);
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));

    setMarginsFromTheme();
    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(slotThemeChanged()));

    setLayout(m_layoutHorizontalTop);
}

void MediaController::setMarginsFromTheme()
{
    qreal left;
    qreal right;
    qreal top;
    qreal bottom;

    m_svg->getMargins(left, top, right, bottom);
    //kDebug() << "setting to" << left << top << right << bottom;
    setContentsMargins(left, top, right, bottom);
}

void MediaController::slotThemeChanged()
{
    m_svg->clearCache();
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));
    setMarginsFromTheme();
}

void MediaController::resetLayouts()
{
    delete m_layoutTopLeft;
    delete m_layoutTopMiddle;
    delete m_layoutTopRight;

    m_layoutTopLeft = new QGraphicsLinearLayout(m_layoutHorizontalTop);
    m_layoutTopMiddle = new QGraphicsLinearLayout(m_layoutHorizontalTop);
    m_layoutTopRight = new QGraphicsLinearLayout(m_layoutHorizontalTop);

    m_layoutTopMiddle->setPreferredWidth(this->size().width()/1.2);
    m_layoutTopMiddle->setContentsMargins(0,0,0,0);
}

void MediaController::addLayouts()
{
    m_layoutHorizontalTop->addItem(m_layoutTopLeft);
    m_layoutHorizontalTop->addItem(m_layoutTopMiddle);
    m_layoutHorizontalTop->addItem(m_layoutTopRight);
}


void MediaController::addToLayout(QGraphicsWidget *widget, const MediaCenter::LayoutZone &zone)
{
    if (zone == MediaCenter::ControlLeft) {
        m_layoutTopLeft->addItem(widget);
    }
    if (zone == MediaCenter::ControlMiddle) {
        m_layoutTopMiddle->addItem(widget);
        m_layoutTopMiddle->setAlignment(widget,Qt::AlignVCenter);
    }
    if (zone == MediaCenter::ControlRight) {
        m_layoutTopRight->addItem(widget);
    }
}

K_EXPORT_PLASMA_APPLET(mediacontroller, MediaController)
