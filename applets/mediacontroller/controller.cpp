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
#include <Phonon/MediaObject>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include <KIcon>
#include <KDebug>

// Plasma
#include <Plasma/IconWidget>
#include <Plasma/Slider>
#include <Plasma/Theme>

MediaController::MediaController(QObject *parent, const QVariantList &args)
    : MediaCenter::PlaybackControl(parent, args),
    m_svg(new Plasma::FrameSvg(this)),

    m_volumeSlider(new Plasma::Slider(this)),
    m_seekSlider(new Plasma::Slider(this)),
    m_iconGoPrevious(new Plasma::IconWidget(this)),
    m_iconTogglePlaylistVisible(new Plasma::IconWidget(this)),
    m_iconToggleControlAutohide(new Plasma::IconWidget(this)),
    m_skipBack(new Plasma::IconWidget(this)),
    m_playPause(new Plasma::IconWidget(this)),
    m_skipForward(new Plasma::IconWidget(this)),
    m_stop(new Plasma::IconWidget(this)),
    m_layout(new QGraphicsLinearLayout(Qt::Horizontal)),
    m_iconSlideshow(new Plasma::IconWidget(this))
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
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

    m_seekSlider->hide();
    m_volumeSlider->hide();

    setLayout(m_layout);
}

void MediaController::playbackStateChanged(MediaCenter::PlaybackState state)
{
    if (state == MediaCenter::PlayingState) {
        m_playPause->setIcon("media-playback-pause");
    } else {
        m_playPause->setIcon("media-playback-start");
    }
}

void MediaController::slotVolumeSlider(int value)
{
    emit volumeLevelChangeRequest((qreal)value / 100);
}

void MediaController::slotSeekSlider(int value)
{
    emit seekRequest(value);
}

void MediaController::receivedMediaObject()
{
    if (!mediaObject()) {
        return;
    }

    m_seekSlider->setRange(0, mediaObject()->totalTime());
    connect (mediaObject(), SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTotalTime(qint64)));
    connect (mediaObject(), SIGNAL(tick(qint64)), this, SLOT(updateCurrentTick(qint64)));

    if (mediaObject()->state() == Phonon::PlayingState) {
        m_playPause->setIcon("media-playback-pause");
    }
}

void MediaController::updateTotalTime(qint64 time)
{
    m_seekSlider->setRange(0, time);
}

void MediaController::updateCurrentTick(qint64 time)
{
    m_seekSlider->setValue(time);
}

void MediaController::setMarginsFromTheme()
{
    qreal left;
    qreal right;
    qreal top;
    qreal bottom;

    m_svg->getMargins(left, top, right, bottom);
    kDebug() << "setting to" << left << top << right << bottom;
    setContentsMargins(left, top, right, bottom);
}

void MediaController::slotThemeChanged()
{
    m_svg->clearCache();
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));
    setMarginsFromTheme();
}

void MediaController::iconAutohidePressed()
{
    //TODO: I need some ON/OFF Effect
}

void MediaController::addToLayout(QList<QGraphicsWidget*> list)
{
    foreach (QGraphicsWidget *w, list) {
        m_layout->addItem(w);
    }
}

K_EXPORT_PLASMA_APPLET(mediacontroller, MediaController)
