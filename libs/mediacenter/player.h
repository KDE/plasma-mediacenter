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
#ifndef PLAYER_H
#define PLAYER_H

#include "mediacenter_export.h"

#include <Plasma/Applet>

#include <mediacenter/mediacenter.h>

namespace Phonon {
    class MediaObject;
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
     * You must reimplement this method in order to return the
     * correct Phonon::MediaObject used to reproduce audio/video
     * content passed to the applet by the MediaContainment that will
     * host it.
     *
     * @note: Note that due to some Phonon limitations you will always
     * have to notify when the current reproduced media type changes to
     * one Phonon does not support, for example a picture (MediaCenter::Picture).
     * The notification should be done via currentTypeChanged(MediaCenter::MediaType) signal.
     */
    virtual Phonon::MediaObject *mediaObject() = 0;

    /**
     * Reimplement this function in order to return the current
     * path to the current reproducing media. If the current media
     * is a MediaCenter::OpticalDisc just return
     * the device name containing the media. Use Solid in order to retrieve it.
     */
    virtual Media currentVideoMedia() const;
    virtual void setCurrentVideoMedia(const Media &media) = 0;
    virtual Media currentPictureMedia() const;
    virtual void setCurrentPictureMedia(const Media &media) = 0;
    virtual Media currentMusicMedia() const;
    virtual void setCurrentMusicMedia(const Media &media) = 0;

    /**
     * Use this method on each state entry to inform the player which type the user wants
     */
    virtual void setPlayerType(const MediaCenter::MediaType &type);
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
     * @see setActive()
     */
    virtual bool isVideoActive();
    virtual bool isMusicActive();
    virtual bool isPictureActive();

    /**
     * This should return the current PLaybackstate of the playerPlaybackState
     */
    virtual MediaCenter::PlaybackState videoPlayerPlaybackState() const = 0;
    virtual MediaCenter::PlaybackState picturePlayerPlaybackState() const = 0;
    virtual MediaCenter::PlaybackState musicPlayerPlaybackState() const = 0;

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
    void activeVideoStateChanged(const bool active);
    void activePictureStateChanged(const bool active);
    void activeMusicStateChanged(const bool active);

    /**
     * Emit this signal when the medialist is empty to prevent
     * the states from showing an empty player
     */
    void nothingToPlay();

    /**
     * This signal should be emitted whenever the playback state
     * changes.
     */
    void videoPlaybackStateChanged(const MediaCenter::PlaybackState);
    void musicPlaybackStateChanged(const MediaCenter::PlaybackState);
    void picturePlaybackStateChanged(const MediaCenter::PlaybackState);

    /**
     * In order to have mediaobject information in the state objects
     * we need these signals. The state need the information in order
     * to update for example the playPause icon or the sliders but also
     * to keep the player uptodate on the current media player wanted
     */
    void newMusicMedia(const MediaCenter::Media &media);
    void newVideoMedia(const MediaCenter::Media &media);
    void newPictureMedia(const MediaCenter::Media &media);
    void newMusicObject(Phonon::MediaObject *object);
    void newVideoObject(Phonon::MediaObject *object);

public Q_SLOTS:
    /**
     * You must reimplement this method in order to set the proper volume.
     */
    virtual void setVolume(const int volume) = 0;
    virtual qreal volume() const = 0;

    virtual void skipVideoForward() = 0;
    virtual void skipVideoBackward() = 0;

    virtual void stopVideo() = 0;
    virtual void playPauseVideo()= 0;

    virtual void skipMusicForward() = 0;
    virtual void skipMusicBackward() = 0;

    virtual void stopMusic() = 0;
    virtual void playPauseMusic() = 0;

    virtual void skipPictureForward() = 0;
    virtual void skipPictureBackward() = 0;

    virtual void stopPicture() = 0;
    virtual void playPausePicture() = 0;

    /**
     * You must reimplement this method in order to let the user
     * seek the media. This method should do anything if the
     * current media is a picture.
     */
    virtual void seekVideo(const int time) = 0;
    virtual void seekMusic(const int time) = 0;

    /**
     * Reimplement this method in order to store and properly set the queue
     * for reproduction.
     *
     * @note: The default implementation does nothing.
     */
    virtual void setVideoQueue(const QList<Media> &sources);
    virtual void setMusicQueue(const QList<Media> &sources);
    virtual void setPictureQueue(const QList<Media> &sources);

    /**
     * Reimplement this method to let the host containment update the queue as needed.
     *
     * @note: The default implementation does nothing.
     */
    virtual void enqueueMusic(const QList<Media> &sources);
    virtual void enqueuePictures(const QList<Media> &sources);
    virtual void enqueueVideos(const QList<Media> &sources);

    /**
     * This function can be used to clear the list of media currently enqueued
     */
    virtual void clearVideoQueue() = 0;
    virtual void clearMusicQueue() = 0;
    virtual void clearPictureQueue() = 0;

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
    virtual void playPictureMedia(const MediaCenter::Media &media);
    virtual void playVideoMedia(const MediaCenter::Media &media);
    virtual void playMusicMedia(const MediaCenter::Media &media);

    /**
     * This function will play all media currently enqued in the playerPlaybackState
     */
    virtual void playAllPictureMedia();
    virtual void playAllVideoMedia();
    virtual void playAllMusicMedia();

protected:
    /**
     * Use this method to set the current state
     * of the player. Active means that the player is playing or
     * paused. Set false when the player is not used, when the queue is empty
     * or when the reproduction is stopped.
     */
    virtual void setVideoActive(const bool set);
    virtual void setMusicActive(const bool set);
    virtual void setPictureActive(const bool set);

private:
    class PlayerPrivate;
    PlayerPrivate *d;
};
}

#endif // PLAYER_H
