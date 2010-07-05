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
#include "videostate.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>
#include <mediacenter/infodisplay.h>
#include <mediacenter/medialayout.h>
#include <mediacenter/private/sharedlayoutcomponentsmanager.h>

#include <nepomuk/kratingwidget.h>
#include <Nepomuk/Resource>
#include <Nepomuk/ResourceManager>

#include <QtCore/QFileInfo>


using namespace MediaCenter;

VideoState::VideoState (QState *parent)
    : MediaCenterState(parent),
    m_videoVolumeSlider(new Plasma::Slider()),
    m_videoSeekSlider(new Plasma::Slider()),
    m_videoSkipBack(new Plasma::IconWidget()),
    m_videoPlayPause(new Plasma::IconWidget()),
    m_videoSkipForward(new Plasma::IconWidget()),
    m_videoStop(new Plasma::IconWidget()),
    m_videoTogglePlaylist(new Plasma::IconWidget()),
    m_selectedMediasLabel(new Plasma::IconWidget()),
    m_currentlyPlayingLabel(new Plasma::IconWidget()),
    m_videoObject(0),
    m_lastDirectory(KUrl()),
    m_resource(0),
    m_ratingWidget(new KRatingWidget()),
    m_ratingProxyWidget(new QGraphicsProxyWidget()),
    m_nepomuk(false)
{
    m_nepomuk = Nepomuk::ResourceManager::instance()->initialized();
}

VideoState::~VideoState()
{
}

void VideoState::onExit(QEvent* event)
{
    Q_UNUSED(event);

    m_lastDirectory = m_browser->directory();

    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
        SharedLayoutComponentsManager::self()->setBackgroundVideoMode(true);
    } else {
        SharedLayoutComponentsManager::self()->setBackgroundVideoMode(false);;
    }

    disconnect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueueVideos(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(playVideoMedia(MediaCenter::Media)));
    disconnect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterPlayingState()));
}

void VideoState::onEntry(QEvent* event)
{
    Q_UNUSED(event);

    emit state(MediaCenter::VideoMode);

    showBackgroundStates();
    //Do not show the current state's background state icon
    SharedLayoutComponentsManager::self()->backgroundVideoWidget()->setVisible(false);

    if (m_lastDirectory.hasPath()) {
         m_browser->openDirectory(m_lastDirectory);
         //TODO: Have the browser load the correct modelpackage
    }

    connect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), this, SLOT(enqueueVideos(QList<MediaCenter::Media>)));
    connect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterPlayingState()));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(playVideoMedia(const MediaCenter::Media&)));
}

void VideoState::playVideoMedia(const MediaCenter::Media &video)
{
    m_player->playMedia(MediaCenter::VideoMode, video);
}

void VideoState::enqueueVideos(const QList<MediaCenter::Media> &videos)
{
    m_player->enqueueMedia(MediaCenter::VideoMode, videos);
}

QList<QGraphicsWidget*> VideoState::subComponents() const
{
    QList<QGraphicsWidget*> list;

    m_videoSkipBack->setIcon("media-skip-backward");
    list << m_videoSkipBack;
    m_control->addToLayout(m_videoSkipBack, MediaCenter::MiddleZone);

    m_videoPlayPause->setIcon("media-playback-start");
    list << m_videoPlayPause;
    m_control->addToLayout(m_videoPlayPause, MediaCenter::MiddleZone);

    m_videoStop->setIcon("media-playback-stop");
    list << m_videoStop;
    m_control->addToLayout(m_videoStop, MediaCenter::MiddleZone);

    m_videoSkipForward->setIcon("media-skip-forward");
    list << m_videoSkipForward;
    m_control->addToLayout(m_videoSkipForward, MediaCenter::MiddleZone);

    m_videoSeekSlider->setRange(0, 100);
    m_videoSeekSlider->setOrientation(Qt::Horizontal);
    list << m_videoSeekSlider;
    m_control->addToLayout(m_videoSeekSlider, MediaCenter::MiddleZone);

    m_videoVolumeSlider->setRange(0, 100);
    m_videoVolumeSlider->setOrientation(Qt::Vertical);
    list << m_videoVolumeSlider;
    m_control->addToLayout(m_videoVolumeSlider, MediaCenter::MiddleZone);

    m_videoTogglePlaylist->setIcon("view-media-playlist");
    list << m_videoTogglePlaylist;
    m_control->addToLayout(m_videoTogglePlaylist, MediaCenter::RightZone);


    list << m_currentlyPlayingLabel;
    m_currentlyPlayingLabel->setMinimumSize(230,20);
    m_currentlyPlayingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_currentlyPlayingLabel, MediaCenter::LeftZone);

    m_ratingProxyWidget->setWidget(m_ratingWidget);
    m_ratingWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_ratingWidget->setMaximumSize(170,35);
    list << m_ratingProxyWidget;
    m_infoDisplay->addToLayout(m_ratingProxyWidget, MediaCenter::MiddleZone);

    list << m_selectedMediasLabel;
    m_selectedMediasLabel->setMinimumSize(230,20);
    m_selectedMediasLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_selectedMediasLabel, MediaCenter::MiddleZone);

    return list;
}

void VideoState::configure()
{
    //Show/hide correct applets
    m_control->setVisible(true);
    m_infoDisplay->setVisible(true);
    if (m_player->playbackState(MediaCenter::VideoMode) != MediaCenter::PlayingState) {
        m_player->setVisible(false);
        m_playlist->setVisible(true);
        m_browser->setVisible(true);
        m_layout->setControlAutohide(false);
        m_layout->setPlaylistVisible(true);

        enterBrowsingState();
   } else {
        m_player->setVisible(true);
        m_playlist->setVisible(true);
        m_browser->setVisible(false);
        m_layout->setControlAutohide(true);
        m_layout->setPlaylistVisible(false);

        enterPlayingState();
    }

    m_browser->clearViewModes();
    m_browser->addViewMode("Videomode 1");
    m_browser->addViewMode("Videomode 2");
    m_browser->setShowingBrowsingWidgets(true);

    //connect seek slider to media object
    setupMediaObject();
    if (m_videoObject) {
        m_videoSeekSlider->setValue(m_videoObject->currentTime());
    }
    m_videoVolumeSlider->setValue(int(m_player->volume(MediaCenter::VideoMode)*100));

    m_player->setCurrentMode(MediaCenter::VideoMode);
    m_player->setCurrentMedia(m_videoMedia, MediaCenter::VideoMode); //the player needs to know what the user wants to control with the buttons
    m_playlist->setPlaylistMediaType(MediaCenter::Video); //TODO: Find a better solution for playlists
    clearVideoQueue();
    m_player->enqueueMedia(MediaCenter::VideoMode, m_playlist->medias(MediaCenter::Video));

    m_browser->clearSelectedMedias();
    updateInfoDisplay();
}

void VideoState::initConnections()
{
    connect (m_videoTogglePlaylist, SIGNAL(clicked()), m_layout, SLOT(togglePlaylistVisible()));
    connect (m_videoSeekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekVideo(int)));
    connect (m_videoStop, SIGNAL(clicked()), this, SLOT(enterBrowsingState()));
    connect (m_videoPlayPause, SIGNAL(clicked()), this, SLOT(playPauseVideo()));
    connect (m_videoSkipForward, SIGNAL(clicked()), this, SLOT(skipVideoForward()));
    connect (m_videoSkipBack, SIGNAL(clicked()), this, SLOT(skipVideoBackward()));
    connect (m_videoVolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setVideoVolume(int)));
    connect (m_player, SIGNAL(newMedia(MediaCenter::Media)), this, SLOT(setMedia(MediaCenter::Media)));
    connect (m_player, SIGNAL(playbackStateChanged(MediaCenter::PlaybackState, MediaCenter::PlaybackState, MediaCenter::Mode)),
             this, SLOT(onPlaybackStateChanged(MediaCenter::PlaybackState, MediaCenter::PlaybackState, MediaCenter::Mode)));
    connect (m_ratingWidget, SIGNAL(ratingChanged(int)), this, SLOT(slotRatingChanged(int)));
}

void VideoState::clearVideoQueue()
{
    m_player->clearQueue(MediaCenter::VideoMode);
}

void VideoState::seekVideo(int time)
{
    m_player->seek(MediaCenter::VideoMode, (qint64)time);
}

void VideoState::skipVideoBackward()
{
    m_player->skipBackward(MediaCenter::VideoMode);
}

void VideoState::skipVideoForward()
{
    m_player->skipForward(MediaCenter::VideoMode);
}

void VideoState::setVideoVolume(int volume)
{
    m_player->setVolume(MediaCenter::VideoMode, volume);
}

void VideoState::updateTotalTime(const qint64 time)
{
    m_videoSeekSlider->setRange(0, time);
}

void VideoState::updateCurrentTick(const qint64 time)
{
    m_videoSeekSlider->setValue(time);
}

void VideoState::onPlaybackStateChanged(MediaCenter::PlaybackState oldState, MediaCenter::PlaybackState state, MediaCenter::Mode mode)
{
    Q_UNUSED(oldState)
    if (mode != MediaCenter::VideoMode) {
        return;
    }
    updateInfoDisplay();

    if (state == MediaCenter::PlayingState) {
        m_videoPlayPause->setIcon("media-playback-pause");
    } else {
        m_videoPlayPause->setIcon("media-playback-start");
    }
}

void VideoState::setMedia(const MediaCenter::Media &media)
{
    if (media.first != MediaCenter::Video) {
        return;
    }
    m_videoMedia = media;
}

void VideoState::setupMediaObject()
{
    m_videoObject = m_player->mediaObject(MediaCenter::VideoMode);
    if (!m_videoObject) {
        kDebug() << "!!!!!!!!!Media object error in VideoState";
        return;
    }
    m_videoSeekSlider->setRange(0, m_videoObject->totalTime());
    connect (m_videoObject, SIGNAL(totalTimeChanged(const qint64)), this, SLOT(updateTotalTime(const qint64)));
    connect (m_videoObject, SIGNAL(tick(const qint64)), this, SLOT(updateCurrentTick(const qint64)));

    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
        m_videoPlayPause->setIcon("media-playback-pause");
    }
}

void VideoState::selectedMediasChanged(const QList<MediaCenter::Media > &list)
{
    Q_UNUSED(list)

    updateInfoDisplay();
}

void VideoState::updateInfoDisplay()
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    //Update rating
    if (list.size() > 1) {
        m_selectedMediasLabel->setText(i18np("Rating of %1 selected item", "Rating of %1 selected items", list.size()));
        m_ratingWidget->setEnabled(true);
    }
    if (list.size() == 1) {
        m_selectedMediasLabel->setText(i18n("Rating of selected item"));
        m_ratingWidget->setEnabled(true);
        m_resource = new Nepomuk::Resource(list[0].second);
        m_ratingWidget->setRating(m_resource->rating());
    }
    if (list.isEmpty()) {
        if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
            m_selectedMediasLabel->setText(i18n("Rating of active item"));
            m_ratingWidget->setEnabled(true);
            m_resource = new Nepomuk::Resource(m_videoMedia.second);
            m_ratingWidget->setRating(m_resource->rating());
        }
        if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::StoppedState) {
            m_selectedMediasLabel->setText("");
            m_ratingWidget->setRating(0);
            m_ratingWidget->setEnabled(false);
        }
    }

    //Update title label
    QString file = m_videoMedia.second;
    //TODO: check if we really have a file
    QString label = QFileInfo(file).fileName();

    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
        m_currentlyPlayingLabel->setText(label);
    }
    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PausedState) {
        m_currentlyPlayingLabel->setText(i18n( "(Paused) %1",label ));
    }
    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::StoppedState) {
        m_currentlyPlayingLabel->setText(i18n("No video playing"));
    }
}

void VideoState::slotRatingChanged(const int rating)
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    if (list.size() > 1) {
        foreach (MediaCenter::Media media, list) {
            m_resource = new Nepomuk::Resource(media.second);
            m_resource->setRating(rating);
        }
    }
    if (list.size() == 1) {
        m_resource = new Nepomuk::Resource(list[0].second);
        m_resource->setRating(rating);
    }
    if (list.isEmpty()) {
        if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
            m_resource = new Nepomuk::Resource(m_videoMedia.second);
            m_resource->setRating(rating);
        }
    }
}

void VideoState::enterBrowsingState() const
{
    m_player->stop(MediaCenter::VideoMode);
    m_layout->showBrowser();
    m_layout->controlAutohideOff();
    m_layout->showPlaylist();
}

void VideoState::enterPlayingState() const
{
    //This function is called every time the play/pause button is clicked
    //We only want to turn autohide on, and hide buttons when the user enters fullscreen player,
    //not when he only pauses or plays when previously paused
    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState ||
        m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::StoppedState) {
        m_layout->controlAutohideOn();
        m_layout->hidePlaylist();
        m_layout->showPlayer();
    }
    m_layout->layoutPlayer();
}

void VideoState::playPauseVideo()
{
    if (m_player->playbackState(MediaCenter::VideoMode) == MediaCenter::StoppedState) {
        enterPlayingState();
    }
    m_player->playPause(MediaCenter::VideoMode);
}
