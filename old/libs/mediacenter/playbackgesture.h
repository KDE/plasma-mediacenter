/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef PLAYBACKGESTURE_H
#define PLAYBACKGESTURE_H

#include <QGesture>

#include "mediacenter_export.h"

namespace MediaCenter {
class MEDIACENTER_EXPORT PlaybackGesture : public QGesture
{
    Q_OBJECT

    Q_PROPERTY(ControlAction activeAction READ activeAction);
    Q_PROPERTY(qreal volumeLevel READ volumeLevel);

public:
    enum ControlAction {
        NoAction,
        PlayPauseAction,
        SkipBackward,
        SkipForward,
        StopAction,
        FastForwardAction,
        RewindAction,
        VolumeAction
    };

    explicit PlaybackGesture(QObject *parent = 0);

    qreal volumeLevel() const;
    ControlAction activeAction() const;

private:
    friend class GestureRecognizer;

    /**
     * Convenient function to increase volume.
     * The volume is increased by the given @param factor
     * divided by 10.
     * volumLevel will never be higher than 1
     * @internal
     */
    void increaseVolume(int factor = 1);

    /**
     * Convenient function to decrease volume.
     * The volume is decreased by the given @param factor
     * divided by 10.
     * volumeLevel will never be less than 0
     */
    void decreaseVolume(int factor = 1);

    ControlAction m_activeAction;
    qreal m_volumeLevel;

};
}
#endif // PLAYBACKGESTURE_H
