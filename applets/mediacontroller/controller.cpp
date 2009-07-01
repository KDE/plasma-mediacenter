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
#include "controller.h"

// Qt
#include <QGraphicsLinearLayout>

#include <KIcon>

// Plasma
#include <Plasma/IconWidget>

MediaController::MediaController(QObject *parent, const QVariantList &args)
    : MediaCenter::PlaybackControl(parent, args),
    m_svg(new Plasma::FrameSvg(this))
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setContentsMargins(0, 0, 0, 0);

    m_svg->setEnabledBorders(Plasma::FrameSvg::LeftBorder | Plasma::FrameSvg::RightBorder | Plasma::FrameSvg::BottomBorder);
    m_svg->setCacheAllRenderedFrames(true);
    m_svg->setImagePath("widgets/background");

    QGraphicsLinearLayout *controlLayout = new QGraphicsLinearLayout(Qt::Horizontal);

    Plasma::IconWidget *skipBack = new Plasma::IconWidget(this);
    skipBack->setIcon("media-skip-backward");
    connect (skipBack, SIGNAL(clicked()), this, SIGNAL(mediaSkipBackwardRequest()));
    controlLayout->addItem(skipBack);

    Plasma::IconWidget *stop = new Plasma::IconWidget(this);
    stop->setIcon("media-playback-stop");
    connect (stop, SIGNAL(clicked()), this, SIGNAL(stopRequest()));
    controlLayout->addItem(stop);

    Plasma::IconWidget *playPause = new Plasma::IconWidget(this);
    playPause->setIcon("media-playback-start");
    // TODO connect
    controlLayout->addItem(playPause);

    Plasma::IconWidget *skipForward = new Plasma::IconWidget(this);
    skipForward->setIcon("media-skip-forward");
    connect (skipForward, SIGNAL(clicked()), this, SIGNAL(mediaSkipForwardRequest()));
    controlLayout->addItem(skipForward);

    setLayout(controlLayout);
}

MediaController::~MediaController()
{}

void MediaController::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints)
    setBackgroundHints(NoBackground);
}

void MediaController::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event)

    m_svg->resizeFrame(contentsRect().size());

//    m_seekBackRect = seekBackRect();
//    m_seekForwardRect = seekForwardRect();
//    m_stopRect = stopRect();
//    m_playPauseRect = playPauseRect();
}

void MediaController::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    m_svg->paintFrame(painter, contentsRect.topLeft());

    // then we draw the controls icon in the following order:
    // seek-back, stop, play/pause, seek-forward
    // using the specified functions to retrieve the correct QRects
    // to draw in.
//    KIcon("media-seek-backward").paint(painter, m_seekBackRect);
//    KIcon("media-playback-stop").paint(painter, m_stopRect);
//    KIcon("media-playback-start").paint(painter, m_playPauseRect);
//    KIcon("media-seek-forward").paint(painter, m_seekForwardRect);
}

int MediaController::iconSizeFromCurrentSize() const
{
    const int size = contentsRect().height();
    return size;
}

QRect MediaController::seekBackRect() const
{
    return QRect(contentsRect().topLeft().toPoint(), QSize(iconSizeFromCurrentSize(), iconSizeFromCurrentSize()));
}

QRect MediaController::stopRect() const
{
    const int size = iconSizeFromCurrentSize();
    QRect rect(contentsRect().topLeft().toPoint(), QSize(size, size));
    rect.translate(size, 0);

    return rect;
}

QRect MediaController::playPauseRect() const
{
    const int size = iconSizeFromCurrentSize();
    QRect rect(contentsRect().topLeft().toPoint(), QSize(size, size));
    rect.translate(size*2, 0);

    return rect;
}

QRect MediaController::seekForwardRect() const
{
    const int size = iconSizeFromCurrentSize();
    QRect rect(contentsRect().topLeft().toPoint(), QSize(size, size));
    rect.translate(size*3, 0);

    return rect;
}


K_EXPORT_PLASMA_APPLET(mediacontroller, MediaController)
