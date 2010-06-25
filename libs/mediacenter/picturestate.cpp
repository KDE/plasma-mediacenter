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

#include "picturestate.h"

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

PictureState::PictureState (QState *parent)
    : MediaCenterState(parent),
    m_pictureToBrowser(new Plasma::IconWidget()),
    m_startSlideshow(new Plasma::IconWidget()),
    m_slideshowTimeSlider(new Plasma::Slider()),
    m_slideshowLabel(new Plasma::IconWidget()),
    m_nextPicture(new Plasma::IconWidget()),
    m_previousPicture(new Plasma::IconWidget()),
    m_selectedMediasLabel(new Plasma::IconWidget()),
    m_currentlyPlayingLabel(new Plasma::IconWidget()),
    m_switchDisplayMode(new Plasma::IconWidget()),
    m_lastDirectory(KUrl()),
    m_ratingWidget(new KRatingWidget()),
    m_ratingProxyWidget(new QGraphicsProxyWidget()),
    m_nepomuk(false)
{
    m_nepomuk = Nepomuk::ResourceManager::instance()->initialized();
}

PictureState::~PictureState()
{
}

void PictureState::onExit(QEvent *event)
{
    Q_UNUSED(event);

    //Pause slideshow if one is playing
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        m_player->playPause(MediaCenter::PictureMode);
    }

    m_lastDirectory = m_browser->directory();

    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PausedState ||
        m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        SharedLayoutComponentsManager::self()->setBackgroundPictureMode(true);
    }
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::StoppedState ||
        m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::SinglePictureState) {
        SharedLayoutComponentsManager::self()->setBackgroundPictureMode(false);
    }

    enterBrowsingState();

    disconnect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(playPictureMedia(const MediaCenter::Media&)));
    disconnect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_layout, SLOT(showPlayer()));
    disconnect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterSinglePictureFullscreenState()));
    disconnect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));
}

void PictureState::playPictureMedia(const MediaCenter::Media &media)
{
    m_player->playMedia(MediaCenter::PictureMode, media);
}

void PictureState::onEntry(QEvent *event)
{
    Q_UNUSED(event);

    emit state(MediaCenter::PictureMode);

    showBackgroundStates();
    SharedLayoutComponentsManager::self()->backgroundPictureWidget()->setVisible(false);

    if (m_lastDirectory.hasPath()) {
        m_browser->openDirectory(m_lastDirectory);
        m_browser->listMediaInDirectory();
    }

    connect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(enterSinglePictureFullscreenState()));
    connect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SLOT(playPictureMedia(const MediaCenter::Media&)));
    connect (m_browser, SIGNAL(mediaActivated(const MediaCenter::Media&)), m_layout, SLOT(showPlayer()));
    connect (m_browser, SIGNAL (selectedMediasChanged(QList<MediaCenter::Media>)), this, SLOT(selectedMediasChanged(QList<MediaCenter::Media>)));

    if (SharedLayoutComponentsManager::self()->isBackgroundPictureMode()) {
        enterSlideshowState();
    } else {
        enterBrowsingState();
    }
}

QList<QGraphicsWidget*> PictureState::subComponents() const
{
    QList<QGraphicsWidget*> list;

    m_previousPicture->setIcon("media-skip-backward");
    list << m_previousPicture;
    m_control->addToLayout(m_previousPicture, MediaCenter::MiddleZone);

    m_nextPicture->setIcon("media-skip-forward");
    list << m_nextPicture;
    m_control->addToLayout(m_nextPicture, MediaCenter::MiddleZone);

    m_startSlideshow->setIcon("media-playback-start");
    list << m_startSlideshow;
    m_control->addToLayout(m_startSlideshow, MediaCenter::RightZone);

    m_slideshowTimeSlider->setOrientation(Qt::Vertical);
    m_slideshowTimeSlider->setRange(0,10);
    m_slideshowTimeSlider->setValue(m_player->slideShowInterval());
    list << m_slideshowTimeSlider;
    m_control->addToLayout(m_slideshowTimeSlider, MediaCenter::RightZone);

    m_slideshowLabel->setText(QString::number(m_player->slideShowInterval()));
    list << m_slideshowLabel;
    m_control->addToLayout(m_slideshowLabel, MediaCenter::RightZone);

    m_pictureToBrowser->setIcon("view-restore");
    list << m_pictureToBrowser;
    m_control->addToLayout(m_pictureToBrowser, MediaCenter::RightZone);

    m_switchDisplayMode->setIcon("view-split-left-right");
    list << m_switchDisplayMode;
    m_control->addToLayout(m_switchDisplayMode, MediaCenter::RightZone);


    list << m_currentlyPlayingLabel;
    m_currentlyPlayingLabel->setMinimumSize(230,20);
    m_currentlyPlayingLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_currentlyPlayingLabel, MediaCenter::LeftZone);

    m_ratingProxyWidget->setWidget(m_ratingWidget);
    m_ratingWidget->setAttribute(Qt::WA_TranslucentBackground);
    m_ratingWidget->setMaximumSize(60,60);
    list << m_ratingProxyWidget;
    m_infoDisplay->addToLayout(m_ratingProxyWidget, MediaCenter::MiddleZone);

    list << m_selectedMediasLabel;
    m_selectedMediasLabel->setMinimumSize(170, 35);
    m_selectedMediasLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_infoDisplay->addToLayout(m_selectedMediasLabel, MediaCenter::MiddleZone);

    return list;
}

void PictureState::configure()
{
    m_layout->setPlaylistVisible(false);
    m_playlist->setVisible(false);

    m_browser->clearViewModes();
    m_browser->addViewMode(i18n("Picturemode 1"));
    m_browser->addViewMode(i18n("Picturemode 2"));
    m_browser->addViewMode(i18n("Picturemode 3"));
    m_browser->setShowingBrowsingWidgets(true);

    m_player->setCurrentMode(MediaCenter::PictureMode);
    clearPictureQueue();
    m_browser->listMediaInDirectory();
}

void PictureState::initConnections()
{
    connect (m_pictureToBrowser, SIGNAL (clicked()), this, SLOT (enterBrowsingState()));
    connect (m_pictureToBrowser, SIGNAL (clicked()), this, SLOT (stopPicture()));
    connect (m_player, SIGNAL(lastPictureShown()), m_pictureToBrowser, SIGNAL(clicked()));
    connect (m_slideshowTimeSlider, SIGNAL(valueChanged(int)), this, SLOT (updateSLideshowInterval(int)));
    connect (m_startSlideshow, SIGNAL (clicked()), this, SLOT (enterSlideshowState()));
    connect (m_browser, SIGNAL (mediasListChanged(QList<MediaCenter::Media>)), this, SLOT (clearPictureQueue()));
    connect (m_browser, SIGNAL (mediasListChanged(QList<MediaCenter::Media>)), this, SLOT (enqueuePictures(QList<MediaCenter::Media>)));
    connect (m_previousPicture, SIGNAL(clicked()), this, SLOT(skipPictureBackward()));
    connect (m_nextPicture, SIGNAL(clicked()), this, SLOT(skipPictureForward()));
    connect (m_previousPicture, SIGNAL(clicked()), this, SLOT(pauseOnSkip()));
    connect (m_nextPicture, SIGNAL(clicked()), this, SLOT(pauseOnSkip()));
    connect (m_player, SIGNAL(playbackStateChanged(MediaCenter::PlaybackState, MediaCenter::Mode)),
             this, SLOT(onPlaybackStateChanged(MediaCenter::PlaybackState, MediaCenter::Mode)));
    connect (m_ratingWidget, SIGNAL(ratingChanged(int)), this, SLOT(slotRatingChanged(int)));
    connect (m_player, SIGNAL(newMedia(MediaCenter::Media)), this, SLOT(setMedia(MediaCenter::Media)));
    connect (m_switchDisplayMode, SIGNAL(clicked()), this, SLOT(toggleFloatingState()));
}

void PictureState::skipPictureBackward()
{
    m_player->skipBackward(MediaCenter::PictureMode);
}

void PictureState::skipPictureForward()
{
    m_player->skipForward(MediaCenter::PictureMode);
}

void PictureState::clearPictureQueue()
{
    m_player->setMediaQueue(MediaCenter::PictureMode, QList<Media>());
}

void PictureState::enqueuePictures(const QList<MediaCenter::Media> &pictures)
{
    m_player->enqueueMedia(MediaCenter::PictureMode, pictures);
}

void PictureState::updateSLideshowInterval(const int &time)
{
    m_slideshowLabel->setText(QString::number(time));
    m_player->setSlideshowInterval(qint64(time));
}

void PictureState::enterBrowsingState()
{
    m_pictureToBrowser->setVisible(false);
    m_nextPicture->setVisible(false);
    m_previousPicture->setVisible(false);
    m_slideshowTimeSlider->setVisible(true);
    m_slideshowLabel->setVisible(true);
    m_startSlideshow->setVisible(true);
    m_switchDisplayMode->setVisible(false);
    m_infoDisplay->setVisible(true);
    SharedLayoutComponentsManager::self()->barAutohideControlWidget()->setVisible(true);

    m_layout->showBrowser();
    m_layout->controlAutohideOff();
    m_layout->setInfoDisplayMode(MediaCenter::InfoDisplayBottom);
    m_infoDisplay->setMode(MediaCenter::InfoDisplayBottom);

    m_player->setSlideshowInterval(m_slideshowTimeSlider->value());

    m_browser->clearSelectedMedias(); //BUG: Since the directory is not changed, the items are still selectedMedias
    updateInfoDisplay();    //BUG: infoDisplay is not repaint, only when I click on it
}

void MediaCenter::PictureState::enterSlideshowState()
{
    if (m_layout->infoDisplayMode() == MediaCenter::InfoDisplayFloating) {
      toggleFloatingState();
    }

    m_player->setSlideshowInterval(m_slideshowTimeSlider->value()); //This needs to be set before any playAll

    if (m_layout->infoDisplayMode() == MediaCenter::InfoDisplayBottom &&
        !SharedLayoutComponentsManager::self()->isBackgroundPictureMode()) {
        m_player->playPause(MediaCenter::PictureMode);
         //If I playAll at end of this function the player does not resize 
         //after clicking play in the browser (=infodisplaybottom mode)
         //thus, I start playing here
    }

    m_pictureToBrowser->setVisible(true);
    m_nextPicture->setVisible(true);
    m_previousPicture->setVisible(true);
    m_slideshowTimeSlider->setVisible(true);
    m_slideshowLabel->setVisible(true);
    m_startSlideshow->setVisible(true);
    m_switchDisplayMode->setVisible(true);

    m_layout->showPlayer();

    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PausedState) {
        m_layout->controlAutohideOff();
    }
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        m_layout->controlAutohideOn();
    }

    m_browser->clearSelectedMedias();
    updateInfoDisplay();

    if (m_layout->infoDisplayMode() == MediaCenter::InfoDisplayFloating &&
        !SharedLayoutComponentsManager::self()->isBackgroundPictureMode()) {
        m_player->playPause(MediaCenter::PictureMode);
      //This playAll is needed to resize player when starting slideshow from floating mode
      //If I put it at the beginning of this function, the player does not resize correctly
      //when starting a slideshow while in floating mode
    }
}

void PictureState::enterSinglePictureFullscreenState()
{
    m_pictureToBrowser->setVisible(true);
    m_nextPicture->setVisible(true);
    m_previousPicture->setVisible(true);
    m_switchDisplayMode->setVisible(true);
    SharedLayoutComponentsManager::self()->barAutohideControlWidget()->setVisible(true);

    m_layout->setInfoDisplayMode(MediaCenter::InfoDisplayBottom);
    m_infoDisplay->setMode(MediaCenter::InfoDisplayBottom);
    m_switchDisplayMode->setIcon("view-split-left-right");

    m_player->setSlideshowInterval(0);

    m_layout->controlAutohideOn();

    m_browser->clearSelectedMedias();
    updateInfoDisplay();

    //Needed to get the player to the correct size if we were in floating previously
    m_layout->layoutPlayer();
}

void PictureState::onPlaybackStateChanged(MediaCenter::PlaybackState state, MediaCenter::Mode mode)
{
    if (mode != MediaCenter::PictureMode) {
        return;
    }
    updateInfoDisplay();

    if (state == MediaCenter::PlayingState) {
        m_startSlideshow->setIcon("media-playback-pause");
        return;
    }

    m_startSlideshow->setIcon("media-playback-start");

}

void MediaCenter::PictureState::pauseOnSkip()
{
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        m_player->playPause(MediaCenter::PictureMode);
    }
}

void PictureState::setMedia(const MediaCenter::Media &media)
{
    if (media.first != MediaCenter::Picture) {
        return;
    }
    m_pictureMedia = media;
    updateInfoDisplay();

    if (m_layout->infoDisplayMode() == MediaCenter::InfoDisplayFloating) {
       m_layout->layoutPlayer();
    }
}

void PictureState::selectedMediasChanged(const QList<MediaCenter::Media > &list)
{
    Q_UNUSED(list)
    updateInfoDisplay();
}

void PictureState::updateInfoDisplay()
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    //Update rating
    if (list.size() > 1) {
        m_selectedMediasLabel->setText(i18np("Rating of %1 selected picture", "Rating of %1 selected pictures", list.size()));
        m_ratingWidget->setEnabled(true);
    }
    if (list.size() == 1) {
        m_selectedMediasLabel->setText(i18n("Rating of selected picture"));
        m_ratingWidget->setEnabled(true);
        Nepomuk::Resource resource(list[0].second);
        m_ratingWidget->setRating(resource.rating());
    }
    if (list.isEmpty()) {
        if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState ||
            m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::SinglePictureState) {
            m_selectedMediasLabel->setText(i18n("Rating of active picture"));
            m_ratingWidget->setEnabled(true);
            Nepomuk::Resource resource(m_pictureMedia.second);
            m_ratingWidget->setRating(resource.rating());
        }
        if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::StoppedState) {
            m_selectedMediasLabel->setText("");
            m_ratingWidget->setRating(0);
            m_ratingWidget->setEnabled(false);
        }
    }

    //Update title label
    QString file = m_pictureMedia.second;
    //TODO:Check for file
    QString label = QFileInfo(file).fileName();

    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        m_currentlyPlayingLabel->setText(label);
    }
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PausedState) {
        m_currentlyPlayingLabel->setText(i18n("(Paused) %1",label ));
    }
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::SinglePictureState) {
        m_currentlyPlayingLabel->setText(i18n("(Single picture) %1",label ));
    }
    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::StoppedState) {
        m_currentlyPlayingLabel->setText(i18n("No picture shown"));
    }
}

void PictureState::slotRatingChanged(const int rating)
{
    QList<MediaCenter::Media> list = m_browser->selectedMedias();

    foreach (MediaCenter::Media media, list) {
        Nepomuk::Resource resource(media.second);
        resource.setRating(rating);
    }

    if (list.isEmpty()) {
        if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PausedState ||
            m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState ||
            m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::SinglePictureState) {
            Nepomuk::Resource resource(m_pictureMedia.second);
            resource.setRating(rating);
        }
    }
}

void PictureState::toggleFloatingState()
{
    if (m_layout->infoDisplayMode() == MediaCenter::InfoDisplayBottom) {
        enterPictureFloatingState();
    } else {
        enterSinglePictureFullscreenState();
    }
}

void PictureState::stopPicture()
{
    if (!m_player) {
        return;
    }

    m_player->stop(MediaCenter::PictureMode);
}

void PictureState::enterPictureFloatingState()
{
    SharedLayoutComponentsManager::self()->barAutohideControlWidget()->setVisible(false);

    m_infoDisplay->setMode(MediaCenter::InfoDisplayFloating);
    m_layout->setInfoDisplayMode(MediaCenter::InfoDisplayFloating);
    m_switchDisplayMode->setIcon("view-preview");

    m_layout->controlAutohideOff();

    if (m_player->playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
        m_player->playPause(MediaCenter::PictureMode);
    }
}
