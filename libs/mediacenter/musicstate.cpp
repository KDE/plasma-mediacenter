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
    m_selectedMediasLabel(new Plasma::IconWidget("")),
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
{}

void MusicState::onExit(QEvent* event)
{
    Q_UNUSED(event);

    m_lastDirectory = m_browser->directory();

    if (m_player->musicPlayerPlaybackState() == MediaCenter::PlayingState) {
        s_backgroundMusicMode = true;
    }
    if (m_player->musicPlayerPlaybackState() == MediaCenter::StoppedState||
        m_player->musicPlayerPlaybackState() == MediaCenter::PausedState) {
        s_backgroundMusicMode = false;
    }

    disconnect (m_browser, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), m_playlist, SLOT(appendMedia(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), m_player, SLOT(enqueueMusic(QList<MediaCenter::Media>)));
    disconnect (m_playlist, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_player, SLOT(playMusicMedia(const MediaCenter::Media&)));
    disconnect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
}

void MusicState::onEntry(QEvent* event)
{
    Q_UNUSED(event);

    emit state(MediaCenter::MusicMode);

    showBackgroundStates();
    s_backgroundMusic->setVisible(false);

    if (m_lastDirectory.hasPath()) {
        m_browser->openDirectory(m_lastDirectory);
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
    m_musicVolumeSlider->setValue(m_player->volume());
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

    m_browser->clearViewModes();
    m_browser->addViewMode("Musicmode 1");
    m_browser->addViewMode("Musicmode 2");
    m_browser->setShowingBrowsingWidgets(true);

    receivedMediaObject();
    if (m_musicObject) {
        m_musicSeekSlider->setValue(m_musicObject->currentTime());
    }
    m_musicVolumeSlider->setValue(int(m_player->volume()*100));

    m_player->setCurrentMusicMedia(m_musicMedia);
    m_player->setPlayerType(MediaCenter::Audio);
    m_playlist->setPlaylistMediaType(MediaCenter::Audio); //TODO: Find a better solution for playlists
    m_player->clearMusicQueue();
    m_player->enqueueMusic(m_playlist->medias(MediaCenter::Audio));

    m_browser->clearSelectedMedias();
    updateInfoDisplay();
}

void MusicState::initConnections()
{
    connect (m_musicTogglePlaylist, SIGNAL(clicked()), m_layout, SLOT(togglePlaylistVisible()));
    connect (m_musicSeekSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(seekMusic(int)));
    connect (m_musicStop, SIGNAL(clicked()), m_player, SLOT(stopMusic()));
    connect (m_musicPlayPause, SIGNAL (clicked()), m_player, SLOT (playAllMusicMedia()));
    connect (m_musicSkipForward, SIGNAL(clicked()), m_player, SLOT(skipMusicForward()));
    connect (m_musicSkipBack, SIGNAL(clicked()), m_player, SLOT(skipMusicBackward()));
    connect (m_musicVolumeSlider, SIGNAL(sliderMoved(int)), m_player, SLOT(setVolume(int)));
    connect (m_player, SIGNAL(newMusicMedia(MediaCenter::Media)), this, SLOT(setMedia(MediaCenter::Media)));
    connect (m_player, SIGNAL(newMusicObject(Phonon::MediaObject*)), this, SLOT(setMediaObject(Phonon::MediaObject*)));
    connect (m_player, SIGNAL(nothingToPlay()), m_musicStop, SIGNAL(clicked()));
    connect (m_player, SIGNAL(musicPlaybackStateChanged(MediaCenter::PlaybackState)), this, SLOT(onPlaybackStateChanged(MediaCenter::PlaybackState)));
    connect (m_ratingWidget, SIGNAL(ratingChanged(int)), this, SLOT(slotRatingChanged(int)));
}

void MusicState::updateTotalTime(const qint64 time)
{
    m_musicSeekSlider->setRange(0, time);
}

void MusicState::updateCurrentTick(const qint64 time)
{
    m_musicSeekSlider->setValue(time);
}

void MusicState::onPlaybackStateChanged(const MediaCenter::PlaybackState &state)
{
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
    m_musicMedia = media;
}

void MusicState::setMediaObject(Phonon::MediaObject *object)
{
    m_musicObject = object;
    receivedMediaObject();
}

Phonon::MediaObject* MusicState::mediaObject() const
{
    return m_musicObject;
}

void MusicState::receivedMediaObject() const
{
    if (!mediaObject()) {
        kDebug() << "media error in MusicState";
        return;
    }
    m_musicSeekSlider->setRange(0, mediaObject()->totalTime());
    connect (mediaObject(), SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTotalTime(qint64)));
    connect (mediaObject(), SIGNAL(tick(qint64)), this, SLOT(updateCurrentTick(qint64)));

    if (m_player->musicPlayerPlaybackState() == MediaCenter::PlayingState) {
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
        QString number = QString::number(list.size());
        m_selectedMediasLabel->setText("Rating of " + number + " selected items");
        m_ratingWidget->setEnabled(true);
    }
    if (list.size() == 1) {
        m_selectedMediasLabel->setText("Rating of selected item");
        m_ratingWidget->setEnabled(true);
        m_resource = new Nepomuk::Resource(list[0].second);
        m_ratingWidget->setRating(m_resource->rating());
    }
    if (list.isEmpty()) {
        if (m_player->musicPlayerPlaybackState() == MediaCenter::PausedState ||
            m_player->musicPlayerPlaybackState() == MediaCenter::PlayingState) {
            m_selectedMediasLabel->setText("Rating of active item");
            m_ratingWidget->setEnabled(true);
            m_resource = new Nepomuk::Resource(m_musicMedia.second);
            m_ratingWidget->setRating(m_resource->rating());
        }
        if (m_player->musicPlayerPlaybackState() == MediaCenter::StoppedState) {
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
                  ref.tag()->title().toCString(true) + QString(" from ") +
                  ref.tag()->artist().toCString(true) + QString(" (") +
                  ref.tag()->album().toCString(true) + QString(")");

    if (m_player->musicPlayerPlaybackState() == MediaCenter::PlayingState) {
        m_currentlyPlayingLabel->setText(label);
    }
    if (m_player->musicPlayerPlaybackState() == MediaCenter::PausedState) {
        m_currentlyPlayingLabel->setText("(Paused) " + label);
    }
    if (m_player->musicPlayerPlaybackState() == MediaCenter::StoppedState) {
        m_currentlyPlayingLabel->setText("No music playing");
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
        if (m_player->musicPlayerPlaybackState() == MediaCenter::PausedState ||
            m_player->musicPlayerPlaybackState() == MediaCenter::PlayingState) {
            m_resource = new Nepomuk::Resource(m_musicMedia.second);
            m_resource->setRating(rating);
        }
    }
}
