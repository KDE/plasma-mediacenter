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
#ifndef PLAYBACKCONTROL_H
#define PLAYBACKCONTROL_H

#include "mediacenter_export.h"
#include "mediacenter.h"

#include <Plasma/Applet>

namespace MediaCenter {

/**
 * @class PlaybackControl playbackcontrol.h
 * @brief Base class for controlling the media reproduction.
 *
 * This is the base class to use when implementing playback controls
 * for the Plasma MediaCenter Components. Keep in mind that most probably
 * this applet will be loaded and put on the topmost edge of the screen when
 * writing the painting code.
 *
 * Always use MediaCenter::MediaObject in order to retrieve current information
 * about media reproduction.
 */

class MEDIACENTER_EXPORT PlaybackControl : public Plasma::Applet
{
public:
    PlaybackControl(QObject *parent, const QVariantList &args);
    virtual ~PlaybackControl();

signals:
    /**
     * Emit this signal whenever the controller receives
     * a play request from the user (e.g. the play button is clicked).
     */
    void playRequest();

    /**
     * Emit this signal whenever pausing the reproduction is requested.
     */
    void pauseRequest();

    /**
     * This signal has to be emitted whenever there is need for seeking
     * the current reproduction.
     * @note: if the current reproduction media is a picture this signal has no effect.
     */
    void seekRequest(qint64 time);

    /**
     * Emit this signal whenever the user requests for the reproduction to be stopped-
     */
    void stopRequest();

    /**
     * Emit this signal whenever the user requests for the volume level to change.
     */
    void volumeLevelChangeRequest(qreal volume);

    /**
     * Emit this signal in order to request the current media to be skipped forward.
     */
    void mediaSkipForwardRequest();

    /**
     * Emit this signal in order to request the current media to be skipped backward.
     */
    void mediaSkipBackwardRequest();

public slots:
    /**
     * This slot is called whenever the current playing state changes.
     * Use this slot in order to update the controller status according
     * to the right playing state.
     * @note: the default implementation actually does nothing.
     */
    virtual void playingStateChanged(MediaCenter::State state);

    /**
     * This slot is automatically called whenever the seek state changes.
     * This way the applet can always keep synced with the current playing
     * state.
     * @note: the default implementation actually does nothing.
     */
    virtual void currentSeekChanged(qreal seek);
};

} // namespace MediaCenter

#endif // PLAYBACKCONTROL_H
