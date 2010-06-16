/***************************************************************************
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>                    *
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

#include "mediaplayer.h"
#include "pictureviewer.h"
//#include "plasmamediaplayeradaptor.h"
//#include "dbus/playerdbushandler.h"
//#include "dbus/rootdbushandler.h"
//#include "dbus/tracklistdbushandler.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QDBusConnection>
#include <QTimer>
#include <QApplication>
#include <QDesktopWidget>

#include <KMimeType>
#include <KFileDialog>
#include <KConfigDialog>

#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>

#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/slider.h>
#include <plasma/widgets/videowidget.h>
#include <Plasma/Containment>

MediaPlayer::MediaPlayer(QObject *parent, const QVariantList &args)
    : MediaCenter::Player(parent, args),
      m_hideTimer(new QTimer(this)),
      m_ticking(false),
      m_raised(false),
      m_fullScreen(false),
      m_sshowTime(3),
      m_picture(new PictureViewer(this)),
      m_currentVideoPlaybackState(MediaCenter::StoppedState),
      m_currentMusicPlaybackState(MediaCenter::StoppedState),
      m_currentPicturePlaybackState(MediaCenter::StoppedState),
      m_currentStateType(MediaCenter::Video)
{
    connect(m_picture, SIGNAL(imageLoaded()), this, SLOT(slotEmitNewMedia()));
    m_picture->hide();
    setAcceptDrops(true);
    setHasConfigurationInterface(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    resize(200, 200);

    if (args.count()) {
        m_currentUrl = args.value(0).toString();
    }
}


MediaPlayer::~MediaPlayer()
{
    delete m_video;
    delete m_music;
}

void MediaPlayer::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget(parent);
    ui.setupUi(widget);

    parent->addPage(widget, i18n("Player settings"), "multimedia-player");

    ui.slideshowSpinBox->setValue(m_sshowTime);
    connect(parent, SIGNAL(accepted()), this, SLOT(acceptConfiguration()));
}

void MediaPlayer::acceptConfiguration()
{
    m_sshowTime = ui.slideshowSpinBox->value();
    applyConfig();
}

void MediaPlayer::loadConfig()
{
    KConfigGroup cf = config();

    m_sshowTime = cf.readEntry("SlideShowTime", 3);
}

void MediaPlayer::applyConfig()
{
    m_picture->setShowTime(m_sshowTime);
    KConfigGroup cf = config();
    cf.writeEntry("SlideShowTime", m_sshowTime);
}

void MediaPlayer::init()
{
    loadConfig();

    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_video = new Plasma::VideoWidget(this);
    m_video->setAcceptDrops(false);

    m_layout->addItem(m_video);

    m_music = new Plasma::VideoWidget(this);
    m_music->setAcceptDrops(false);

    m_layout->addItem(m_music);

    Phonon::MediaObject *media = m_video->mediaObject();

    media->setTickInterval(200);

    Phonon::MediaObject *audio = m_music->mediaObject();

    audio->setTickInterval(200);

    SetControlsVisible(false);

    connect (m_video->mediaObject(), SIGNAL(finished()), this, SLOT(playNextVideoMedia()));
    connect (m_music->mediaObject(), SIGNAL(finished()), this, SLOT(playNextMusicMedia()));
    connect (m_picture, SIGNAL(showFinished()), this, SLOT(playNextPictureMedia()));

    m_picture->setShowTime(slideShowInterval());
    connect (this, SIGNAL(slideShowTimeChanged(qint64)), m_picture, SLOT(setShowTime(qint64)));
}

void MediaPlayer::playNextPictureMedia()
{
    int nextMedia = m_pictureMedias.indexOf(currentMedia(MediaCenter::PictureMode)) + 1;
    if (nextMedia >= m_pictureMedias.count()) {
        setModeActive(MediaCenter::PictureMode, false);
        return;
    }
    if (m_currentPicturePlaybackState == MediaCenter::PlayingState ||
        m_currentPicturePlaybackState == MediaCenter::StoppedState ||
        m_currentPicturePlaybackState == MediaCenter::PausedState ||
        m_currentPicturePlaybackState == MediaCenter::SinglePictureState) {
        playPictureMedia(m_pictureMedias[nextMedia]);
    }
}

void MediaPlayer::playNextVideoMedia()
{
    int nextMedia = m_videoMedias.indexOf(currentMedia(MediaCenter::VideoMode)) + 1;
    if (nextMedia >= m_videoMedias.count()) {
        setModeActive(MediaCenter::VideoMode, false);
        return;
    }
    if (m_currentVideoPlaybackState == MediaCenter::PlayingState) {
        playVideoMedia(m_videoMedias[nextMedia]);
    }
}

void MediaPlayer::playNextMusicMedia()
{
    int nextMedia = m_musicMedias.indexOf(currentMedia(MediaCenter::MusicMode)) + 1;
    if (nextMedia >= m_musicMedias.count()) {
        setModeActive(MediaCenter::MusicMode, false);
        return;
    }
    if (m_currentMusicPlaybackState == MediaCenter::PlayingState) {
        playMusicMedia(m_musicMedias[nextMedia]);
    }
}

void MediaPlayer::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);

    setBackgroundHints(NoBackground);
}

void MediaPlayer::SetControlsVisible(bool visible)
{
    m_video->setControlsVisible(visible);
    m_music->setControlsVisible(visible);
}

bool MediaPlayer::ControlsVisible() const
{
    return (m_video->controlsVisible() && m_music->controlsVisible());
}

void MediaPlayer::ToggleControlsVisibility()
{
    SetControlsVisible(!m_video->controlsVisible());
    SetControlsVisible(!m_music->controlsVisible());
}

void MediaPlayer::playPauseVideo()
{
    Phonon::MediaObject *media = m_video->mediaObject();
    if (media->state() == Phonon::PlayingState) {
        media->pause();
        m_currentVideoPlaybackState = MediaCenter::PausedState;
        emit videoPlaybackStateChanged(MediaCenter::PausedState);
    } else {
        media->play();
        m_currentVideoPlaybackState = MediaCenter::PlayingState;
        emit videoPlaybackStateChanged(MediaCenter::PlayingState);
    }
}

void MediaPlayer::playPauseMusic()
{
    Phonon::MediaObject *media = m_music->mediaObject();
    if (media->state() == Phonon::PlayingState) {
        media->pause();
        m_currentMusicPlaybackState = MediaCenter::PausedState;
        emit musicPlaybackStateChanged(MediaCenter::PausedState);
    } else {
        media->play();
        m_currentMusicPlaybackState = MediaCenter::PlayingState;
        emit musicPlaybackStateChanged(MediaCenter::PlayingState);
    }
}

void MediaPlayer::playPausePicture()
{
    if (m_currentPicturePlaybackState == MediaCenter::SinglePictureState) {
        m_picture->startTimer();
        m_currentPicturePlaybackState = MediaCenter::PlayingState;
        emit picturePlaybackStateChanged(MediaCenter::PlayingState);
        playPictureMedia(currentMedia(MediaCenter::PictureMode));
        return;
    }
    if (m_picture->isTimerActive()) {
        m_picture->stopTimer();
        m_currentPicturePlaybackState = MediaCenter::PausedState;
        emit picturePlaybackStateChanged(MediaCenter::PausedState);
        return;
    }
    if (!m_picture->isTimerActive() ||
        m_currentPicturePlaybackState == MediaCenter::SinglePictureState) {
        m_picture->startTimer();
        m_currentPicturePlaybackState = MediaCenter::PlayingState;
        emit picturePlaybackStateChanged(MediaCenter::PlayingState);
    }
}

void MediaPlayer::RaiseLower()
{
    if (m_raised) {
        Lower();
        m_raised = false;
    } else {
        Raise();
        m_raised = true;
    }
}

void MediaPlayer::Raise()
{
    Raise();
    m_raised = true;
}

void MediaPlayer::Lower()
{
    Lower();
    m_raised = true;
}

void MediaPlayer::seekVideo(int progress)
{
    if (!m_ticking) {
        if (m_currentStateType == MediaCenter::Video) {
            m_video->mediaObject()->seek(progress);
        }
    }
}

void MediaPlayer::seekMusic(int progress)
{
    if (!m_ticking) {
        if (m_currentStateType == MediaCenter::Audio) {
            m_music->mediaObject()->seek(progress);
        }
    }
}

void MediaPlayer::setVolume(int value)
{
    //They should all have the same volume.
    m_video->audioOutput()->setVolume(qreal(value)/100);
    m_music->audioOutput()->setVolume(qreal(value)/100);
}

qreal MediaPlayer::volume() const
{
    return m_video->audioOutput()->volume();
}


void MediaPlayer::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->urls().isEmpty()) {
        return;
    }

    QList<MediaCenter::Media> medias;
    foreach (const KUrl &url, event->mimeData()->urls()) {
        if (!QFile::exists(url.path())) {
            continue;
        }

        MediaCenter::MediaType type = MediaCenter::getType(url.path());
        if (type != MediaCenter::Invalid) {
            MediaCenter::Media media;
            media.first = type;
            media.second = url.path();
            medias << media;
        }
    }

    emit mediaReceived(medias);
}

void MediaPlayer::ShowOpenFileDialog()
{
    OpenUrl(KFileDialog::getOpenFileName());
}

void MediaPlayer::OpenUrl(const QString &url)
{
    m_currentUrl = url;
    m_video->setUrl(m_currentUrl);
    m_video->mediaObject()->play();
}

void MediaPlayer::hideControls()
{
    SetControlsVisible(false);
}

void MediaPlayer::keyPressEvent(QKeyEvent *event)
{
    Phonon::MediaObject *media;
    Phonon::AudioOutput *audio;
    const qreal step = 30;

    if (m_currentStateType == MediaCenter::Video) {
        media = m_video->mediaObject();
        audio = m_video->audioOutput();
    }
    if (m_currentStateType == MediaCenter::Audio) {
        media = m_music->mediaObject();
        audio = m_music->audioOutput();
    }

    switch (event->key())
    {
    case Qt::Key_Left:
        media->seek(media->currentTime() - media->totalTime()/step);
        break;
    case Qt::Key_Right:
        media->seek(media->currentTime() + media->totalTime()/step);
        break;
    case Qt::Key_Space:
        if (m_currentStateType == MediaCenter::Audio) {
            playPauseMusic();
        }
        if (m_currentStateType == MediaCenter::Video) {
            playPauseVideo();
        }
        if (m_currentStateType == MediaCenter::Picture) {
            playPausePicture();
        }
        break;
    case Qt::Key_Up:
        audio->setVolume(qMin(qreal(1.0), audio->volume() + 0.1));
        break;
    case Qt::Key_Down:
        audio->setVolume(qMax(qreal(0.0), audio->volume() - 0.1));
        break;
    case Qt::Key_R:
        RaiseLower();
        break;
    case Qt::Key_V:
        ToggleControlsVisibility();
        break;
    default:
        break;
    }

    if (event->key() != Qt::Key_V) {
        SetControlsVisible(true);
#warning "Implement this!"
        m_hideTimer->start(2000);
    }
}

bool MediaPlayer::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);

    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_Escape) {
            m_fullScreen = false;
            return true;
        }
    }
    return false;
}

void MediaPlayer::stopVideo()
{
    if (m_currentStateType == MediaCenter::Video) {
        m_video->mediaObject()->stop();
        m_currentVideoPlaybackState = MediaCenter::StoppedState;
        emit videoPlaybackStateChanged(MediaCenter::StoppedState);
    }
    setModeActive(MediaCenter::VideoMode, false);
}

void MediaPlayer::stopMusic()
{
    if (m_currentStateType == MediaCenter::Audio) {
        m_music->mediaObject()->stop();
        m_currentMusicPlaybackState = MediaCenter::StoppedState;
        emit musicPlaybackStateChanged(MediaCenter::StoppedState);
    }
    setModeActive(MediaCenter::MusicMode, false);
}

void MediaPlayer::stopPicture()
{
    if (m_currentStateType == MediaCenter::Picture) {
        m_picture->clearImage();
        m_picture->stopTimer();
        m_currentPicturePlaybackState = MediaCenter::StoppedState;
        emit picturePlaybackStateChanged(MediaCenter::StoppedState);
    }
    setModeActive(MediaCenter::PictureMode, false);
}

Phonon::MediaObject* MediaPlayer::mediaObject()
{
    if (m_currentStateType == MediaCenter::Video) {
        return m_video->mediaObject();
    }
    if (m_currentStateType == MediaCenter::Audio) {
        return m_music->mediaObject();
    }
    return 0;
}

void MediaPlayer::skipVideoBackward()
{
    int previous = m_videoMedias.indexOf(currentMedia(MediaCenter::VideoMode)) - 1;
    if (previous < 0) {
        return;
    }
    if (m_currentVideoPlaybackState == MediaCenter::PlayingState) {
        playVideoMedia(m_videoMedias[previous]);
    }
}

void MediaPlayer::skipVideoForward()
{
    playNextVideoMedia();
}

void MediaPlayer::skipMusicBackward()
{
    int previous = m_musicMedias.indexOf(currentMedia(MediaCenter::MusicMode)) - 1;
    if (previous < 0) {
        return;
    }
    if (m_currentMusicPlaybackState == MediaCenter::PlayingState) {
        playMusicMedia(m_musicMedias[previous]);
    }
}

void MediaPlayer::skipMusicForward()
{
    playNextMusicMedia();
}

void MediaPlayer::skipPictureBackward()
{
    int previous = m_pictureMedias.indexOf(currentMedia(MediaCenter::PictureMode)) - 1;
    if (previous < 0) {
        return;
    }
    if (m_currentPicturePlaybackState == MediaCenter::PlayingState ||
        m_currentPicturePlaybackState == MediaCenter::PausedState ||
        m_currentPicturePlaybackState == MediaCenter::SinglePictureState) {
        playPictureMedia(m_pictureMedias[previous]);
    }
}

void MediaPlayer::skipPictureForward()
{
    playNextPictureMedia();
}

void MediaPlayer::playVideoMedia(const MediaCenter::Media &media)
{
    foreach (MediaCenter::Media mediaSource, m_videoMedias)  {
        if (mediaSource.second == media.second) {
            setModeActive(MediaCenter::VideoMode, true);
            //Video
            if (media.first == MediaCenter::Video ||
                media.first == MediaCenter::OpticalDisc) {
                // check whether there is the right widget in the layout
                if (!m_video->isVisible()) {
                    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
                    layout->removeItem(m_picture);
                    layout->removeItem(m_music);
                    m_video->show();
                    m_music->hide();
                    m_picture->hide();
                    layout->addItem(m_video);
                    setLayout(layout);
                }
                m_video->mediaObject()->setCurrentSource(media.second);

                if (m_video->mediaObject()->state() != Phonon::PlayingState) {
                    setCurrentMedia(media, MediaCenter::VideoMode);
                    emit newMedia(media);
                    emit newVideoObject(m_video->mediaObject());
                    playPauseVideo();
                }
                return;
            }
        }
    }
}

void MediaPlayer::playMusicMedia(const MediaCenter::Media &media)
{
    foreach (MediaCenter::Media mediaSource, m_musicMedias)  {
        if (mediaSource.second == media.second) {
            setModeActive(MediaCenter::MusicMode, true);
            //Audio
            if (media.first == MediaCenter::Audio) {
                // check whether there is the right widget in the layout
                if (!m_music->isVisible()) {
                    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
                    layout->removeItem(m_picture);
                    layout->removeItem(m_video);
                    m_music->show();
                    m_video->hide();
                    m_picture->hide();
                    layout->addItem(m_music);
                    setLayout(layout);
                }
                m_music->mediaObject()->setCurrentSource(media.second);

                if (m_music->mediaObject()->state() != Phonon::PlayingState) {
                    setCurrentMedia(media, MediaCenter::MusicMode);
                    emit newMedia(media);
                    emit newMusicObject(m_music->mediaObject());
                    playPauseMusic();
                }
                return;
            }
        }
    }
}
void MediaPlayer::playPictureMedia(const MediaCenter::Media &media)
{
    foreach (const MediaCenter::Media &mediaSource, m_pictureMedias)  {
        if (mediaSource.second == media.second) {
            setModeActive(MediaCenter::PictureMode, true);
            //Picture
            if (media.first == MediaCenter::Picture) {
                slideShow(media);
                break;
            } else {
                kWarning() << "MediaType unknown";
            }
        } else {
            kDebug() << "media not found";
        }
    }
}

void MediaPlayer::slideShow(const MediaCenter::Media &media)
{
    if (!m_picture->isVisible()) {
        QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
        layout->removeItem(m_video);
        layout->removeItem(m_music);
        m_video->hide();
        m_music->hide();
        m_picture->show();
        layout->addItem(m_picture);
        setLayout(layout);
    }
    kDebug() << "loading picture" << media.second;
    m_picture->loadPicture(media.second);
    setCurrentMedia(media, MediaCenter::PictureMode);

    if (slideShowInterval() == 0 && !m_picture->isTimerActive()) {
       m_currentPicturePlaybackState = MediaCenter::SinglePictureState;
       emit picturePlaybackStateChanged(MediaCenter::SinglePictureState);
    }

    if (slideShowInterval() > 0 || m_picture->isTimerActive()) {
        if (m_currentPicturePlaybackState == MediaCenter::PlayingState) {
            m_picture->startTimer();
            disconnect(m_picture, SIGNAL(showFinished()), this, SIGNAL(lastPictureShown()));
            disconnect(this, SIGNAL(lastPictureShown()), this, SLOT(returnToFirstPicture()));

            if (m_pictureMedias.indexOf(media) == m_pictureMedias.size() -1 ) {
                //The picture state exits slideshow to return to the nrowser
                //after receiving this signal, also, if the user starts a new slideshow
                //it will start again from the first picture
                connect(m_picture, SIGNAL(showFinished()), this, SIGNAL(lastPictureShown()));
                connect(this, SIGNAL(lastPictureShown()), this, SLOT(returnToFirstPicture()));
            }
        }
    }
}

void MediaPlayer::appendMusic(const QStringList &medias)
{
    foreach (const QString mediaString, medias) {
        MediaCenter::Media media;
        media.first = MediaCenter::getType(mediaString);
        media.second = mediaString;

        kDebug() << "appending" << mediaString;
        m_musicMedias << media;
    }
}

void MediaPlayer::appendPicture(const QStringList &medias)
{
    foreach (const QString mediaString, medias) {
        MediaCenter::Media media;
        media.first = MediaCenter::getType(mediaString);
        media.second = mediaString;

        kDebug() << "appending" << mediaString;
        m_pictureMedias << media;
    }
}

void MediaPlayer::appendVideo(const QStringList &medias)
{
    foreach (const QString mediaString, medias) {
        MediaCenter::Media media;
        media.first = MediaCenter::getType(mediaString);
        media.second = mediaString;

        kDebug() << "appending" << mediaString;
        m_videoMedias << media;
    }
}

void MediaPlayer::enqueueVideos(const QList<MediaCenter::Media> &medias)
{
    m_videoMedias << medias;
}

void MediaPlayer::enqueuePictures(const QList<MediaCenter::Media> &medias)
{
    m_pictureMedias << medias;
}

void MediaPlayer::enqueueMusic(const QList<MediaCenter::Media> &medias)
{
    m_musicMedias << medias;
}

void MediaPlayer::clearPictureQueue()
{
    m_pictureMedias.clear();
}

void MediaPlayer::clearVideoQueue()
{
    m_videoMedias.clear();
}

void MediaPlayer::clearMusicQueue()
{
    m_musicMedias.clear();
}

MediaCenter::PlaybackState MediaPlayer::videoPlayerPlaybackState() const
{
    return m_currentVideoPlaybackState;
}

MediaCenter::PlaybackState MediaPlayer::musicPlayerPlaybackState() const
{
    return m_currentMusicPlaybackState;
}

MediaCenter::PlaybackState MediaPlayer::picturePlayerPlaybackState() const
{
    return m_currentPicturePlaybackState;
}

void MediaPlayer::playAllVideoMedia()
{
    if (m_videoMedias.isEmpty()) {
        kWarning() << "No video to play";
        emit nothingToPlay();
        //TODO: Do something with this signal
        return;
    }

    if (m_currentVideoPlaybackState == MediaCenter::PlayingState) {
        playPauseVideo();
        return;
    }

    MediaCenter::Media media;
    if (m_videoMedias.contains(currentMedia(MediaCenter::VideoMode))) {
        media = currentMedia(MediaCenter::VideoMode);
    } else {
        media = m_videoMedias[0];
    }

    if (m_currentStateType == MediaCenter::Video) {
        if (m_currentVideoPlaybackState == MediaCenter::PausedState) {
            playPauseVideo();
            return;
        }
        if (m_currentVideoPlaybackState == MediaCenter::StoppedState) {
            playVideoMedia(media);
            playPauseVideo();
            return;
        }
    }
}

void MediaPlayer::playAllMusicMedia()
{
    if (m_musicMedias.isEmpty()) {
        kWarning() << "No music to play";
        emit nothingToPlay();
        //TODO: Do something with this signal
        return;
    }

    if (m_currentMusicPlaybackState == MediaCenter::PlayingState) {
        playPauseMusic();
        return;
    }

    MediaCenter::Media media;
    if (m_musicMedias.contains(currentMedia(MediaCenter::MusicMode))) {
        media = currentMedia(MediaCenter::MusicMode);
    } else {
        media = m_musicMedias[0];
    }

    if (m_currentStateType == MediaCenter::Audio) {
        if (m_currentMusicPlaybackState == MediaCenter::PausedState) {
            playPauseMusic();
            return;
        }
        if (m_currentMusicPlaybackState == MediaCenter::StoppedState) {
            playMusicMedia(media);
            playPauseMusic();
            return;
        }
    }
}

void MediaPlayer::playAllPictureMedia()
{
    if (m_pictureMedias.isEmpty()) {
        kWarning() << "No picture to play";
        emit nothingToPlay();
        //TODO: Do something with this signal
        return;
    }

    if (m_currentPicturePlaybackState == MediaCenter::PlayingState) {
        playPausePicture();
        return;
    }

    MediaCenter::Media media;
    if (m_pictureMedias.contains(currentMedia(MediaCenter::PictureMode))) {
        media = currentMedia(MediaCenter::PictureMode);
    } else {
        media = m_pictureMedias[0];
    }

    if (m_currentStateType == MediaCenter::Picture) {
        if (m_currentPicturePlaybackState == MediaCenter::PausedState) {
            playPictureMedia(currentMedia(MediaCenter::PictureMode));
            playPausePicture();
            return;
        }
        if (m_currentPicturePlaybackState == MediaCenter::StoppedState ||
            m_currentPicturePlaybackState == MediaCenter::SinglePictureState) {
            playPictureMedia(media);
            playPausePicture();
            return;
        }
    }
}

void MediaPlayer::setPlayerType(const MediaCenter::MediaType &type)
{
    if (type == MediaCenter::Audio) {
        m_video->hide();
        m_music->hide();
        m_picture->hide();
    }
    if (type == MediaCenter::Video) {
        m_video->show();
        m_music->hide();
        m_picture->hide();
    }
    if (type == MediaCenter::Picture) {
        m_video->hide();
        m_music->hide();
        m_picture->show();
    }
    m_currentStateType = type;
}

QRectF MediaPlayer::pictureRect() const
{
    return m_picture->pictureRect();
}

void MediaPlayer::returnToFirstPicture()
{
    setCurrentMedia(m_pictureMedias.first(), MediaCenter::PictureMode); //For next slideshow to start at beginning
}

void MediaPlayer::slotEmitNewMedia()
{
    emit newMedia(currentMedia(MediaCenter::PictureMode));
}

K_EXPORT_PLASMA_APPLET(mcplayer, MediaPlayer)

#include "mediaplayer.moc"
