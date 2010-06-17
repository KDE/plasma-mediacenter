/***************************************************************************
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

#ifndef VIDEOSTATE_H
#define VIDEOSTATE_H

#include "mediacenterstate.h"

#include <phonon/mediaobject.h>

#include <Plasma/IconWidget>
#include <Plasma/Slider>

namespace Nepomuk {
    class Resource;
}

class KRatingWidget;

namespace MediaCenter {

class MEDIACENTER_EXPORT VideoState : public MediaCenterState
{
    Q_OBJECT
public:
    VideoState(QState *parent = 0);
    virtual ~VideoState();

    virtual QList<QGraphicsWidget*> subComponents() const;
    virtual void configure();
    virtual void initConnections();

    /**
     * Use this method to get the current Phonon::MediaObject used in the player of that state.
     * We use this object to update the seek slider
     */
    Phonon::MediaObject* mediaObject() const;

protected:
    virtual void onExit(QEvent* event);
    virtual void onEntry(QEvent* event);

private slots:
    void updateTotalTime(const qint64 time);
    void updateCurrentTick(const qint64 time);

    /**
     * This slot is called whenever the current playing state changes.
     * Use this slot in order to update the controller status according
     * to the right playing state.
     */
    virtual void onPlaybackStateChanged(MediaCenter::PlaybackState, MediaCenter::Mode);

    void setMedia(const MediaCenter::Media &media);

   /**
     * This method is connected to the player and updates
     * the mediaObject currently active in the state
     */
    void setMediaObject(Phonon::MediaObject *object);

    /**
     * Currently the browser emits a signal when the number of selected items changes
     * This slot is connected to that signal and updates the information in the state
     */
    void selectedMediasChanged(const QList<MediaCenter::Media> &list);

    void slotRatingChanged(const int rating);

    /**
     * Call this function when the player is shown, to for example hide unnecessary buttons
     */
    void enterPlayingState() const;

    /**
     * Call this function when the browser is shown, to for example hide unnecessary buttons
     */
    void enterBrowsingState() const;

private:

    /**
     * This method is called whenever a new Phonon::MediaObject is set
     * via setMediaObject(). Use this method for internal update purposes
     * when the MediaObject is ready.
     */
    void receivedMediaObject() const;

    void updateInfoDisplay();

    Plasma::Slider *m_videoVolumeSlider;
    Plasma::Slider *m_videoSeekSlider;
    Plasma::IconWidget *m_videoSkipBack;
    Plasma::IconWidget *m_videoPlayPause;
    Plasma::IconWidget *m_videoSkipForward;
    Plasma::IconWidget *m_videoStop;
    Plasma::IconWidget *m_videoTogglePlaylist;
    Plasma::IconWidget *m_selectedMediasLabel;
    Plasma::IconWidget *m_currentlyPlayingLabel;

    Phonon::MediaObject *m_videoObject;
    MediaCenter::Media m_videoMedia;
    KUrl m_lastDirectory;

    Nepomuk::Resource *m_resource;
    KRatingWidget *m_ratingWidget;
    QGraphicsProxyWidget *m_ratingProxyWidget;
    bool m_nepomuk;
};

} //namespace end
#endif // VIDEOSTATE_H
