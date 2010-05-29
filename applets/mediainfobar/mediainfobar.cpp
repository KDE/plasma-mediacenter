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
#include "mediainfobar.h"

// Qt
#include <QGraphicsLinearLayout>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <KIcon>
#include <KDebug>

// Plasma
#include <Plasma/Theme>

MediaInfoBar::MediaInfoBar(QObject *parent, const QVariantList &args)
    : MediaCenter::InfoDisplay(parent, args),
    m_svg(new Plasma::FrameSvg(this)),
    m_layoutHorizontalTop(new QGraphicsLinearLayout(this)),
    m_layoutTopLeft(new QGraphicsLinearLayout(Qt::Horizontal, m_layoutHorizontalTop)),
    m_layoutTopMiddle(new QGraphicsLinearLayout(Qt::Horizontal, m_layoutHorizontalTop)),
    m_layoutTopRight(new QGraphicsLinearLayout(Qt::Horizontal, m_layoutHorizontalTop)),
    m_currentMode(MediaCenter::InfoDisplayBottom)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setContentsMargins(0,0,0,0);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
}

MediaInfoBar::~MediaInfoBar()
{}

void MediaInfoBar::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    setBackgroundHints(NoBackground);
    setMarginsFromTheme();
}

void MediaInfoBar::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event)

    m_svg->resizeFrame(rect().size());
}

void MediaInfoBar::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    m_svg->paintFrame(painter, option->rect);
}

int MediaInfoBar::iconSizeFromCurrentSize() const
{
    const int size = contentsRect().height();
    return size;
}

void MediaInfoBar::init()
{
    m_svg->setEnabledBorders(Plasma::FrameSvg::TopBorder);
    m_svg->setCacheAllRenderedFrames(true);
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));

    setMarginsFromTheme();
    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(slotThemeChanged()));

    setLayout(m_layoutHorizontalTop);
}

void MediaInfoBar::setMarginsFromTheme()
{
    qreal left;
    qreal right;
    qreal top;
    qreal bottom;

    m_svg->getMargins(left, top, right, bottom);
    //kDebug() << "setting to" << left << top << right << bottom;
    setContentsMargins(left, top, right, bottom);
}

void MediaInfoBar::slotThemeChanged()
{
    m_svg->clearCache();
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));
    setMarginsFromTheme();
}

void MediaInfoBar::resetLayout()
{
    delete m_layoutTopLeft;
    delete m_layoutTopMiddle;
    delete m_layoutTopRight;

    m_layoutTopLeft = new QGraphicsLinearLayout(m_layoutHorizontalTop);
    m_layoutTopMiddle = new QGraphicsLinearLayout(m_layoutHorizontalTop);
    m_layoutTopRight = new QGraphicsLinearLayout(m_layoutHorizontalTop);
}

void MediaInfoBar::applyLayout()
{
    m_layoutHorizontalTop->addItem(m_layoutTopLeft);
    m_layoutHorizontalTop->addItem(m_layoutTopMiddle);
    m_layoutHorizontalTop->addItem(m_layoutTopRight);
}


void MediaInfoBar::addToLayout(QGraphicsWidget *widget, const MediaCenter::LayoutZone &zone)
{
    if (zone == MediaCenter::LeftZone) {
        m_layoutTopLeft->addItem(widget);
    }
    if (zone == MediaCenter::MiddleZone) {
        m_layoutTopMiddle->addItem(widget);
    }
    if (zone == MediaCenter::RightZone) {
        m_layoutTopRight->addItem(widget);
    }
}

void MediaInfoBar::setMode(const MediaCenter::InfoDisplayMode &mode)
{
    if (m_currentMode == mode) {
        return;
    }
    m_currentMode = mode;

    if (mode == MediaCenter::InfoDisplayBottom) {
        m_svg->setEnabledBorders(Plasma::FrameSvg::TopBorder);
        m_layoutTopMiddle->setOrientation(Qt::Horizontal);
        m_layoutHorizontalTop->setOrientation(Qt::Horizontal);
    }
    if (mode == MediaCenter::InfoDisplayFloating) {
        m_svg->setEnabledBorders(Plasma::FrameSvg::AllBorders);
        m_layoutTopMiddle->setOrientation(Qt::Vertical);
        m_layoutHorizontalTop->setOrientation(Qt::Vertical);
    }
    m_layoutHorizontalTop->invalidate();
    m_layoutTopLeft->invalidate();
    m_layoutTopMiddle->invalidate();
    m_layoutTopRight->invalidate();
}

K_EXPORT_PLASMA_APPLET(mediainfobar, MediaInfoBar)
