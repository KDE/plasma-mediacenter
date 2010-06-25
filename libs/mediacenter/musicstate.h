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

#ifndef MUSICSTATE_H
#define MUSICSTATE_H

#include "mediacenterstate.h"

#include <phonon/mediaobject.h>

#include <Plasma/IconWidget>
#include <Plasma/Slider>

namespace Nepomuk {
    class Resource;
}

class KRatingWidget;

namespace MediaCenter {

class MEDIACENTER_EXPORT MusicState : public MediaCenterState
{
    Q_OBJECT
public:
    MusicState(QState *parent = 0);
    virtual ~MusicState();

    virtual QList<QGraphicsWidget*> subComponents() const;
    virtual void configure();
    virtual void initConnections();

protected:
    virtual void onExit(QEvent* event);
    virtual void onEntry(QEvent* event);

private slots:
    void updateTotalTime(const qint64 time);
    void updateCurrentTick(const qint64 time);

    virtual void onPlaybackStateChanged(MediaCenter::PlaybackState state, MediaCenter::Mode);

    void setMedia(const MediaCenter::Media &media);

    void selectedMediasChanged(const QList<MediaCenter::Media> &list);
    void slotRatingChanged(const int rating);

    void enqueueMusicMedia(const QList<MediaCenter::Media> &medias);

    void stopMusic();

    void playPauseMusicQueue();
    void seekMusic(int);
    void skipMusicForward();
    void skipMusicBackward();
    void clearMusicQueue();
    void setMusicVolume(int);

private:
    void setupMediaObject();
    void updateInfoDisplay();

    Plasma::Slider *m_musicVolumeSlider;
    Plasma::Slider *m_musicSeekSlider;
    Plasma::IconWidget *m_musicSkipBack;
    Plasma::IconWidget *m_musicPlayPause;
    Plasma::IconWidget *m_musicSkipForward;
    Plasma::IconWidget *m_musicStop;
    Plasma::IconWidget *m_musicTogglePlaylist;
    Plasma::IconWidget *m_selectedMediasLabel;
    Plasma::IconWidget *m_currentlyPlayingLabel;

    Phonon::MediaObject *m_musicObject;
    MediaCenter::Media m_musicMedia;
    KUrl m_lastDirectory;

    Nepomuk::Resource *m_resource;
    KRatingWidget *m_ratingWidget;
    QGraphicsProxyWidget *m_ratingProxyWidget;
    bool m_nepomuk;

};

} //namespace end
#endif // MUSICSTATE_H
