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

#include <mediacenter/playbackcontrol.h>

#include <phonon/phononnamespace.h>

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

public slots:
    void playbackStateChanged(MediaCenter::State);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);
    void receivedMediaObject();

private slots:
    void slotVolumeSlider(int value);
    void slotSeekSlider(int value);
    void updateTotalTime(qint64 time);
    void updateCurrentTick(qint64 time);
    void slotThemeChanged();

private:
    Plasma::FrameSvg *m_svg;
    Plasma::IconWidget *m_playPause;
    Plasma::Slider *m_volumeSlider;
    Plasma::Slider *m_seekSlider;

    int iconSizeFromCurrentSize() const;

    void setMarginsFromTheme();
};

#endif
