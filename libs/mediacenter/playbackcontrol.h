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

namespace Phonon {
    class MediaObject;
}

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
    Q_OBJECT
public:
    PlaybackControl(QObject *parent, const QVariantList &args);
    virtual ~PlaybackControl();

    /**
     * This method is called from the containment in order to set the
     * proper Phonon::MediaObject currently in use to reproduce media content.
     */
    void setMediaObject(Phonon::MediaObject *mediaObject);

    /**
     * Use this method to retrieve the current Phonon::MediaObject in use.
     * Calling this method in the constructor might not be safe
     * since a MediaObject is associated to the applet only after the applet
     * has been added to the containment. You might want to store the
     * pointer in the init() function.
     */
    Phonon::MediaObject *mediaObject();

Q_SIGNALS:
    /**
     * Emit this signal whenever the controller receives
     * a play/pause request from the user (e.g. the play button is clicked).
     */
    void playPauseRequest();

    /**
     * This signal has to be emitted whenever there is need for seeking
     * the current reproduction.
     * @note: if the current reproduction media is a picture this signal has no effect.
     */
    void seekRequest(int time);

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

public Q_SLOTS:
    /**
     * This slot is called whenever the current playing state changes.
     * Use this slot in order to update the controller status according
     * to the right playing state.
     * @note: the default implementation actually does nothing.
     */
    virtual void playbackStateChanged(MediaCenter::State state);

    /**
     * This slot is automatically called whenever the seek state changes.
     * This way the applet can always keep synced with the current playing
     * state.
     * @note: the default implementation actually does nothing.
     */
    virtual void currentSeekChanged(qreal seek);

    /**
     * This slot is called whenever a new MediaType is going to be
     * reproduced (or showed in case of pictures). You might want to
     * reimplement this slot in order to disable for example
     * seek and volume controls when a picture is being showed.
     */
    virtual void mediaTypeChanged(MediaCenter::MediaType newType);

protected:
    /**
     * This method is called whenever a new Phonon::MediaObject is set
     * via setMediaObject(). Use this method for internal update purposes
     * when the MediaObject is ready.
     *
     * @note: the default implementation does nothing.
     */
    virtual void receivedMediaObject();

private:
    class PlaybackControlPrivate;
    PlaybackControlPrivate *d;
};

} // namespace MediaCenter

#endif // PLAYBACKCONTROL_H
