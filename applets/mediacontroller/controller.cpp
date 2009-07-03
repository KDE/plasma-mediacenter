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
#include <Phonon/MediaObject>

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

    m_playPause = new Plasma::IconWidget(this);
    m_playPause->setIcon("media-playback-start");
    // TODO connect
    controlLayout->addItem(m_playPause);

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
}

void MediaController::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    m_svg->paintFrame(painter, contentsRect.topLeft());
}

int MediaController::iconSizeFromCurrentSize() const
{
    const int size = contentsRect().height();
    return size;
}

void MediaController::init()
{
    m_mediaObject = mediaObject();

    if (!m_mediaObject) {
        return;
    }
    if (m_mediaObject->state() == Phonon::PlayingState) {
        m_playPause->setIcon("media-playback-pause");
    }
}

void MediaController::togglePlayPayse(Phonon::State oldState, Phonon::State newState)
{
    Q_UNUSED(oldState);

    if (newState == Phonon::PlayingState) {
        m_playPause->setIcon("media-playback-pause");
    } else {
        m_playPause->setIcon("media-playback-start");
    }
}


K_EXPORT_PLASMA_APPLET(mediacontroller, MediaController)
