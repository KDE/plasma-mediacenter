/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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
#ifndef PLAYER_H
#define PLAYER_H

#include "mediacenter_export.h"

#include <Plasma/Applet>

#include <mediacenter/mediacenterstate.h>
#include <mediacenter/mediacenter.h>

namespace Phonon {
    class MediaObject;
    class AudioOutput;
}

namespace MediaCenter {

/**
 * @class Player player.h
 * @brief Is the main class for handling the reproduction of all MediaTypes.
 *
 * This class is the main class that should be reimplemented when writing
 * a mediaplayer applet for the MediaCenter. Every supported MediaCenter::MediaType
 * has to be handled by this class.
 */

class MEDIACENTER_EXPORT Player : public Plasma::Applet
{
    Q_OBJECT
public:
    Player(QObject *parent, const QVariantList &args);
    virtual ~Player();

    /**
     * @return the Phonon::MediaObject for the given @param mode.
     * If no MediaObject is used for the given @param mode this method
     * returns 0.
     *
     * @note: Note that due to the internal design you'll always have to
     * notify when the current media type changes.
     * This should be done via currentTypeChanged(MediaCenter::MediaType) signal.
     */
    virtual Phonon::MediaObject *mediaObject(MediaCenter::Mode);

    /**
     * @return the current media for the given @param mode
     * or an invalid media if no current media is found.
     */
    Media currentMedia(MediaCenter::Mode) const;

    /**
     * Sets the current media for the given @param mode
     */
    virtual void setCurrentMedia(const Media &media, MediaCenter::Mode mode);

    /**
     * Use this method on each state entry to inform the player
     * which type of media control are currently available for.
     */
    virtual void setCurrentMode(MediaCenter::Mode mode);

    MediaCenter::Mode currentMode() const;

    /**
     * This method is used to set the slideshow interval when
     * showing queues of pictures.
     */
    void setSlideshowInterval(const qint64 time);
    qint64 slideShowInterval() const;

    virtual QRectF pictureRect() const = 0;

    /**
     * @return the current state of the player.
     * @internal
     * @see setActiveMode()
     */
    virtual bool isModeActive(MediaCenter::Mode) const;

    /**
     * @return the playback state for the given @param mode
     */
    MediaCenter::PlaybackState playbackState(MediaCenter::Mode mode) const;

    /**
     * Sets the playback state to @param state for the given @param mode
     */
    void setPlaybackState(MediaCenter::PlaybackState state, MediaCenter::Mode mode);

    /**
     * @return the enqueued media list for the given @param mode
     */
    QList<Media> mediaQueue(MediaCenter::Mode mode) const;

Q_SIGNALS:
    /**
     * Take care of emitting this signal whenever the reproduced
     * MediaType changes.
     *
     * @note: Note that the MediaContainment will use this signal
     * to inform every applet that the current MediaType has changed and
     * will completely ignore the currentSourceChanged() from the Phonon::MediaObject
     * returned by mediaObject().
     */
    void currentTypeChanged(const MediaCenter::MediaType &newType);

    /**
     * This signal should be emitted whenever the user
     * directly tries to reproduce media via the player.
     * This could happen for example when the user drops media files
     * on the player instead of the playlist.
     *
     * @param medias: the list of media received by the user.
     */
    void mediaReceived(const QList<MediaCenter::Media> &medias);

    /**
     * This signal is emitted whenever the slide show time changes.
     */
    void slideShowTimeChanged(const qint64 time);

    /**
     * This signal is emitted whenever the active state changes.
     */
    void activeModeChanged(MediaCenter::Mode mode, bool active);

    /**
     * Emit this signal when the medialist is empty to prevent
     * the states from showing an empty player
     */
    void nothingToPlay();

    /**
     * This signal is emitted each time the state for the specific mode changes.
     */
    void playbackStateChanged(MediaCenter::PlaybackState, MediaCenter::Mode);

    void lastPictureShown();

    /**
     * In order to have mediaobject information in the state objects
     * we need these signals. The state need the information in order
     * to update for example the playPause icon or the sliders but also
     * to keep the player uptodate on the current media player wanted
     */
    void newMedia(const MediaCenter::Media &media);

public Q_SLOTS:
    /**
     * Convenience function to set the volume.
     * @param volume must be between 0 and 100.
     */
    virtual void setVolume(MediaCenter::Mode, int volume);
    virtual qreal volume(MediaCenter::Mode);

    virtual void skipForward(MediaCenter::Mode) = 0;
    virtual void skipBackward(MediaCenter::Mode) = 0;

    virtual void stop(MediaCenter::Mode) = 0;
    virtual void playPause(MediaCenter::Mode) = 0;

    /**
     * This method seeks the MediaObject for the given @param mode
     * by @param time
     */
    virtual void seek(MediaCenter::Mode mode, qint64 time);

    /**
     * Reimplement this method in order to store and properly set the queue
     * for reproduction.
     *
     * @note: The default implementation does nothing.
     */
    virtual void setMediaQueue(MediaCenter::Mode, const QList<Media> &medias);

    /**
     * Reimplement this method to let the host containment update the queue as needed.
     */
    virtual void enqueueMedia(MediaCenter::Mode, const QList<Media> &medias);

    /**
     * This function can be used to clear the list of media currently
     * enqueued for the given @param mode.
     */
    virtual void clearQueue(MediaCenter::Mode mode);

//    /**
//     * This slot is called when one or more media are going to be
//     * enqueued at a specified index.
//     */
//    virtual void enqueue(int index, const QList<Media> &sources);

    /**
     * This slot should be called whenever it is needed to make
     * the player play the specified media.
     *
     * @param media is the media to be played.
     * @note the default implementation does nothing.
     */
    virtual void playMedia(MediaCenter::Mode, const MediaCenter::Media &media) = 0;

protected:
    /**
     * Use this method to set the current state
     * of the player. Active means that the player is playing or
     * paused. Set false when the player is not used, when the queue is empty
     * or when the reproduction is stopped.
     */
    void setModeActive(MediaCenter::Mode mode, bool set);

    /**
     * @return the Phonon::AudioOutput associated with the given
     * @param mode
     *
     * @note always use Phonon::createPath with the Phonon::AudioOutput
     * returned by this function.
     */
    Phonon::AudioOutput *audioOutput(MediaCenter::Mode mode);

private:
    class PlayerPrivate;
    PlayerPrivate *d;
};
}

#endif // PLAYER_H
