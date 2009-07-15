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
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <playbackcontrol.h>

#include <phonon/phononnamespace.h>

namespace Phonon {
    class MediaObject;
}

namespace Plasma {
    class IconWidget;
    class Slider;
}

class MediaController : public MediaCenter::PlaybackControl
{
    Q_OBJECT
public:
     MediaController(QObject *parent, const QVariantList &args);
    ~MediaController();

    void constraintsEvent(Plasma::Constraints constraints);

    void paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect);

    void init();

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void receivedMediaObject();

private slots:
    void togglePlayPause(Phonon::State, Phonon::State);
    void slotVolumeSlider(int value);
    void slotSeekSlider(int value);

private:
    Plasma::FrameSvg *m_svg;
    Phonon::MediaObject *m_mediaObject;
    Plasma::IconWidget *m_playPause;
    Plasma::Slider *m_volumeSlider;
    Plasma::Slider *m_seekSlider;

    int iconSizeFromCurrentSize() const;

    inline QRect seekBackRect() const;
    inline QRect seekForwardRect() const;
    inline QRect stopRect() const;
    inline QRect playPauseRect() const;


};

#endif
