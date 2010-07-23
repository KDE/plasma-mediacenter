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

#include "musicstate.h"

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

// Taglib
#include <taglib/fileref.h>
#include <taglib/tag.h>


using namespace MediaCenter;

MusicState::MusicState (QState *parent)
    : MediaCenterState(parent),
    m_musicVolumeSlider(new Plasma::Slider()),
    m_musicSeekSlider(new Plasma::Slider()),
    m_musicSkipBack(new Plasma::IconWidget()),
    m_musicPlayPause(new Plasma::IconWidget()),
    m_musicSkipForward(new Plasma::IconWidget()),
    m_musicStop(new Plasma::IconWidget()),
    m_musicTogglePlaylist(new Plasma::IconWidget()),
    m_selectedMediasLabel(new Plasma::IconWidget()),
    m_currentlyPlayingLabel(new Plasma::IconWidget()),
    m_musicObject(new Phonon::MediaObject()),
    m_lastDirectory(KUrl()),
    m_resource(0),
    m_ratingWidget(new KRatingWidget()),
    m_ratingProxyWidget(new QGraphicsProxyWidget()),
    m_nepomuk(false)
{
    m_nepomuk = Nepomuk::ResourceManager::instance()->initialized();
}

MusicState::~MusicState()
{
}

void MusicState::onExit(QEvent* event)
{
    Q_UNUSED(event);

    m_lastDirectory = m_browser->currentUrl();

    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PlayingState) {
        SharedLayoutComponentsManager::self()->setBackgroundMusicMode(true);
    }
    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::StoppedState||
        m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PausedState) {
        SharedLayoutComponentsManager::self()->setBackgroundMusicMode(false);
    }

    disconnect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), this, SLOT(enqueueMusicMedia(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playMusicMedia(const MediaCenter::Media&)));
    disconnect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
}

void MusicState::enqueueMusicMedia(const QList<MediaCenter::Media> &medias)
{
    if (!m_player) {
        return;
    }

    m_player->enqueueMedia(MediaCenter::MusicMode, medias);
}

void MusicState::onEntry(QEvent* event)
{
    Q_UNUSED(event);

    emit state(MediaCenter::MusicMode);

    showBackgroundStates();
    SharedLayoutComponentsManager::self()->backgroundMusicWidget()->setVisible(false);

    if (m_lastDirectory.hasPath()) {
        m_browser->openUrl(m_lastDirectory);
    }

    connect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueueMusic(QList<MediaCenter::Media>)));
    connect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playMusicMedia(const MediaCenter::Media&)));
    connect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
}

QList<QGraphicsWidget*> MusicState::subComponents() const
{
    QList<QGraphicsWidget*> list;

    m_musicSkipBack->setIcon("media-skip-backward");
    list << m_musicSkipBack;
    m_control->addToLayout(m_musicSkipBack, MediaCenter::MiddleZone);

    m_musicPlayPause->setIcon("media-playback-start");
    list << m_musicPlayPause;
    m_control->addToLayout(m_musicPlayPause, MediaCenter::MiddleZone);

    m_musicStop->setIcon("media-playback-stop");
    list << m_musicStop;
    m_control->addToLayout(m_musicStop, MediaCenter::MiddleZone);

    m_musicSkipForward->setIcon("media-skip-forward");
    list << m_musicSkipForward;
    m_control->addToLayout(m_musicSkipForward, MediaCenter::MiddleZone);

    m_musicSeekSlider->setRange(0, 100);
    m_musicSeekSlider->setOrientation(Qt::Horizontal);
    list << m_musicSeekSlider;
    m_control->addToLayout(m_musicSeekSlider, MediaCenter::MiddleZone);

    m_musicVolumeSlider->setRange(0, 100);
    m_musicVolumeSlider->setOrientation(Qt::Vertical);
    m_musicVolumeSlider->setValue(m_player->volume(MediaCenter::MusicMode)*100);
    list << m_musicVolumeSlider;
    m_control->addToLayout(m_musicVolumeSlider, MediaCenter::MiddleZone);

    m_musicTogglePlaylist->setIcon("view-media-playlist");
    list << m_musicTogglePlaylist;
    m_control->addToLayout(m_musicTogglePlaylist, MediaCenter::RightZone);

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

void MusicState::configure()
{
    m_control->setVisible(true);
    m_infoDisplay->setVisible(true);
    m_player->setVisible(false);
    m_playlist->setVisible(true);
    m_browser->setVisible(true);
    m_layout->setControlAutohide(false);
    m_layout->setPlaylistVisible(true);

    m_layout->controlAutohideOff();

    m_browser->setEnableToolbar(true);

    setupMediaObject();
    if (m_musicObject) {
        m_musicSeekSlider->setValue(m_musicObject->currentTime());
    }
    m_musicVolumeSlider->setValue(int(m_player->volume(MediaCenter::VideoMode)*100));

    m_player->setCurrentMedia(m_musicMedia, MediaCenter::MusicMode);
    m_player->setCurrentMode(MediaCenter::MusicMode);
    m_playlist->setPlaylistMediaType(MediaCenter::Audio); //TODO: Find a better solution for playlists
    clearMusicQueue();
    m_player->enqueueMedia(MediaCenter::MusicMode, m_playlist->medias(MediaCenter::Audio));

    m_browser->clearSelectedMedias();
    updateInfoDisplay();
}

void MusicState::initConnections()
{
    connect (m_musicTogglePlaylist, SIGNAL(clicked()), m_layout, SLOT(togglePlaylistVisible()));
    connect (m_musicSeekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekMusic(int)));
    connect (m_musicStop, SIGNAL(clicked()), this, SLOT(stopMusic()));
    connect (m_musicPlayPause, SIGNAL (clicked()), this, SLOT (playPauseMusicQueue()));
    connect (m_musicSkipForward, SIGNAL(clicked()), this, SLOT(skipMusicForward()));
    connect (m_musicSkipBack, SIGNAL(clicked()), this, SLOT(skipMusicBackward()));
    connect (m_musicVolumeSlider, SIGNAL(sliderMoved(int)), this, SLOT(setMusicVolume(int)));
    connect (m_player, SIGNAL(newMedia(MediaCenter::Media)), this, SLOT(setMedia(MediaCenter::Media)));
    connect (m_player, SIGNAL(nothingToPlay()), m_musicStop, SIGNAL(clicked()));
    connect (m_player, SIGNAL(playbackStateChanged(MediaCenter::PlaybackState, MediaCenter::PlaybackState, MediaCenter::Mode)),
             this, SLOT(onPlaybackStateChanged(MediaCenter::PlaybackState, MediaCenter::PlaybackState, MediaCenter::Mode)));
    connect (m_ratingWidget, SIGNAL(ratingChanged(int)), this, SLOT(slotRatingChanged(int)));
}

void MusicState::playPauseMusicQueue()
{
    m_player->playPause(MediaCenter::MusicMode);
}

void MusicState::seekMusic(int time)
{
    m_player->seek(MediaCenter::MusicMode, (qint64)time);
}

void MusicState::skipMusicBackward()
{
    m_player->skipBackward(MediaCenter::MusicMode);
}

void MusicState::skipMusicForward()
{
    m_player->skipForward(MediaCenter::MusicMode);
}

void MusicState::clearMusicQueue()
{
    m_player->clearQueue(MediaCenter::MusicMode);
}

void MusicState::setMusicVolume(int volume)
{
    m_player->setVolume(MediaCenter::MusicMode, volume);
}

void MusicState::updateTotalTime(const qint64 time)
{
    m_musicSeekSlider->setRange(0, time);
}

void MusicState::updateCurrentTick(const qint64 time)
{
    m_musicSeekSlider->setValue(time);
}

void MusicState::onPlaybackStateChanged(MediaCenter::PlaybackState oldState, MediaCenter::PlaybackState state, MediaCenter::Mode mode)
{
    Q_UNUSED(oldState)
    if (mode != MediaCenter::MusicMode) {
        return;
    }
    updateInfoDisplay();

    if (state == MediaCenter::PlayingState) {
        m_musicPlayPause->setIcon("media-playback-pause");
        return;
    } else {
        m_musicPlayPause->setIcon("media-playback-start");
    }
}

void MusicState::setMedia(const MediaCenter::Media &media)
{
    if (media.first != MediaCenter::Audio) {
        return;
    }
    m_musicMedia = media;
}

void MusicState::setupMediaObject()
{
    m_musicObject = m_player->mediaObject(MediaCenter::MusicMode);
    if (!m_musicObject) {
        kDebug() << "mediaObject error in MusicState";
        return;
    }
    m_musicSeekSlider->setRange(0, m_musicObject->totalTime());
    connect(m_musicObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTotalTime(qint64)));
    connect(m_musicObject, SIGNAL(tick(qint64)), this, SLOT(updateCurrentTick(qint64)));

    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PlayingState) {
        m_musicPlayPause->setIcon("media-playback-pause");
    }
}

void MusicState::selectedMediasChanged(const QList<MediaCenter::Media > &list)
{
    updateInfoDisplay();
}

void MusicState::updateInfoDisplay()
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
        if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PlayingState) {
            m_selectedMediasLabel->setText(i18n("Rating of active item"));
            m_ratingWidget->setEnabled(true);
            m_resource = new Nepomuk::Resource(m_musicMedia.second);
            m_ratingWidget->setRating(m_resource->rating());
        }
        if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::StoppedState) {
            m_selectedMediasLabel->setText("");
            m_ratingWidget->setRating(0);
            m_ratingWidget->setEnabled(false);
        }
    }


    //Update artist/album label
    QString file = m_musicMedia.second;
    //TODO: Check for file here
    TagLib::FileRef ref(file.toLatin1());
    QString label = ref.isNull() ? QFileInfo(file).fileName() :
                  i18nc("'Track title' from 'track author' (track album)",
                        "%1 from %2 (%3)",
                        ref.tag()->title().toCString(true),
                        ref.tag()->artist().toCString(true),
                        ref.tag()->album().toCString(true)
                  );

    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PlayingState) {
        m_currentlyPlayingLabel->setText(label);
    }
    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PausedState) {
        m_currentlyPlayingLabel->setText(i18n("(Paused) %1", label));
    }
    if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::StoppedState) {
        m_currentlyPlayingLabel->setText(i18n("No music playing"));
    }
}

void MusicState::slotRatingChanged(const int rating)
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
        if (m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::MusicMode) == MediaCenter::PlayingState) {
            m_resource = new Nepomuk::Resource(m_musicMedia.second);
            m_resource->setRating(rating);
        }
    }
}

void MusicState::stopMusic()
{
    if (!m_player) {
        return;
    }

    m_player->stop(MediaCenter::MusicMode);
}
