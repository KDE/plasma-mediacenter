/***************************************************************************
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>                    *
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

#include "mediaplayer.h"
#include "pictureviewer.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QTimer>
#include <QGraphicsView>
#include <QFile>

#include <KConfigDialog>
#include <KWindowSystem>

#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>

#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/slider.h>
#include <plasma/widgets/videowidget.h>
#include <Plasma/Containment>
#include <Plasma/Wallpaper>
#include <Plasma/WindowEffects>

MediaPlayer::MediaPlayer(QObject *parent, const QVariantList &args)
    : MediaCenter::Player(parent, args),
      m_video(0),
      m_music(0),
      m_hideTimer(new QTimer(this)),
      m_raised(false),
      m_fullScreen(false),
      m_sshowTime(3),
      m_picture(new PictureViewer(this)),
      m_mainWidget(0),
      m_videoProxy(new QGraphicsProxyWidget(this))
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


    view()->installEventFilter(this);

    // let's try to find the main window
    QObject *parent = view()->parent();
    while (1) {
        if (parent->parent()) {
            parent = parent->parent();
            continue;
        }
        break;
    }
    parent->installEventFilter(this);
    m_mainWidget = qobject_cast<QWidget*>(parent);

    m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_video = new Phonon::VideoWidget;
    m_videoProxy->setWidget(m_video);
    m_video->hide();

    Phonon::createPath(mediaObject(MediaCenter::VideoMode), m_video);

    m_music = mediaObject(MediaCenter::MusicMode);

    mediaObject(MediaCenter::VideoMode)->setTickInterval(200);

    m_music->setTickInterval(200);

    connect (mediaObject(MediaCenter::VideoMode), SIGNAL(finished()), this, SLOT(playNextVideoMedia()));
    connect (m_music, SIGNAL(finished()), this, SLOT(playNextMusicMedia()));
    connect (m_picture, SIGNAL(showFinished()), this, SLOT(playNextPictureMedia()));
    connect(this, SIGNAL(playbackStateChanged(MediaCenter::PlaybackState, MediaCenter::PlaybackState,MediaCenter::Mode)),
            SLOT(reactToStateChange(MediaCenter::PlaybackState, MediaCenter::PlaybackState,MediaCenter::Mode)));

    m_picture->setShowTime(slideShowInterval());
    connect (this, SIGNAL(slideShowTimeChanged(qint64)), m_picture, SLOT(setShowTime(qint64)));
    connect(KWindowSystem::self(), SIGNAL(activeWindowChanged(WId)), this, SLOT(adjustActiveWindow(WId)));
}

void MediaPlayer::playNextMusicMedia()
{
    playNextMedia(MediaCenter::MusicMode);
}

void MediaPlayer::playNextPictureMedia()
{
    playNextMedia(MediaCenter::PictureMode);
}

void MediaPlayer::playNextVideoMedia()
{
    playNextMedia(MediaCenter::VideoMode);
}

void MediaPlayer::playNextMedia(MediaCenter::Mode mode)
{
    const int nextMedia = mediaQueue(mode).indexOf(currentMedia(mode)) + 1;
    if (nextMedia >= mediaQueue(mode).count()) {
        setModeActive(mode, false);
        return;
    }

    playMedia(mode, mediaQueue(mode).at(nextMedia));
}

void MediaPlayer::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);

    setBackgroundHints(NoBackground);
}

void MediaPlayer::playPause(MediaCenter::Mode mode)
{
    if (mode == MediaCenter::VideoMode) {
        if (playbackState(MediaCenter::VideoMode) == MediaCenter::PlayingState) {
            mediaObject(MediaCenter::VideoMode)->pause();
            setPlaybackState(MediaCenter::PausedState, mode);
        } else if (playbackState(MediaCenter::VideoMode) == MediaCenter::PausedState){
            mediaObject(MediaCenter::VideoMode)->play();
            setPlaybackState(MediaCenter::PlayingState, mode);
        } else {
            mediaObject(MediaCenter::VideoMode)->setCurrentSource(currentMedia(MediaCenter::VideoMode).second);
            mediaObject(MediaCenter::VideoMode)->play();
            setPlaybackState(MediaCenter::PlayingState, MediaCenter::VideoMode);
        }
    } else if (mode == MediaCenter::MusicMode) {
        if (m_music->state() == Phonon::PlayingState) {
            m_music->pause();
            setPlaybackState(MediaCenter::PausedState, mode);
        } else if (m_music->state() == Phonon::PausedState) {
            m_music->play();
            setPlaybackState(MediaCenter::PlayingState, mode);
        }
    } else if (mode == MediaCenter::PictureMode) {
        if (playbackState(MediaCenter::PictureMode) == MediaCenter::StoppedState) {
            m_picture->startTimer();
            setPlaybackState(MediaCenter::PlayingState, MediaCenter::PictureMode);
            playMedia(mode, currentMedia(mode));
        } else if (m_picture->isTimerActive()) {
            m_picture->stopTimer();
            setPlaybackState(MediaCenter::PausedState, MediaCenter::PictureMode);
        } else if (!m_picture->isTimerActive() ||
            playbackState(MediaCenter::PictureMode) == MediaCenter::StoppedState) {
            m_picture->startTimer();
            setPlaybackState(MediaCenter::PlayingState, MediaCenter::PictureMode);
        }
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

void MediaPlayer::keyPressEvent(QKeyEvent *event)
{
    Phonon::MediaObject *media = mediaObject(currentMode());
    Phonon::AudioOutput *audio = audioOutput(currentMode());
    const qreal step = 30;

    switch (event->key())
    {
    case Qt::Key_Left:
        if (media) {
            media->seek(media->currentTime() - media->totalTime()/step);
        }
        break;
    case Qt::Key_Right:
        if (media) {
            media->seek(media->currentTime() + media->totalTime()/step);
        }
        break;
    case Qt::Key_Space:
        if (currentMode() == MediaCenter::MusicMode) {
            playPause(MediaCenter::MusicMode);
        }
        if (currentMode() == MediaCenter::VideoMode) {
            playPause(MediaCenter::VideoMode);
        }
        if (currentMode() == MediaCenter::PictureMode) {
            playPause(MediaCenter::PictureMode);
        }
        break;
    case Qt::Key_Up:
        if (audio) {
            audio->setVolume(qMin(qreal(1.0), audio->volume() + 0.1));
        }
        break;
    case Qt::Key_Down:
        if (audio) {
            audio->setVolume(qMax(qreal(0.0), audio->volume() - 0.1));
        }
        break;
    case Qt::Key_R:
        RaiseLower();
        break;
    case Qt::Key_V:
        break;
    default:
        break;
    }
}

bool MediaPlayer::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::Resize || e->type() == QEvent::Move) {
        updateVideoWidgetGeometry();
    }
    return false;
}

void MediaPlayer::stop(MediaCenter::Mode mode)
{
    if (mode == MediaCenter::VideoMode) {
        mediaObject(mode)->stop();
        setPlaybackState(MediaCenter::StoppedState, MediaCenter::VideoMode);
        setModeActive(MediaCenter::VideoMode, false);
    } else if (mode == MediaCenter::MusicMode) {
        m_music->stop();
        setPlaybackState(MediaCenter::StoppedState, MediaCenter::MusicMode);
        setModeActive(MediaCenter::MusicMode, false);
    } else if (mode == MediaCenter::PictureMode) {
        m_picture->clearImage();
        m_picture->stopTimer();
        setPlaybackState(MediaCenter::StoppedState, MediaCenter::PictureMode);
        setModeActive(MediaCenter::PictureMode, false);
    }
}

void MediaPlayer::skipBackward(MediaCenter::Mode mode)
{
    const int previous = mediaQueue(mode).indexOf(currentMedia(mode)) - 1;
    playMedia(mode, mediaQueue(mode).at(previous));
}

void MediaPlayer::skipForward(MediaCenter::Mode mode)
{
    playNextMedia(mode);
}

void MediaPlayer::playMedia(MediaCenter::Mode mode, const MediaCenter::Media &media)
{
    // the given media was not previously enqueued
    // we shouldn't be here
    Q_ASSERT(mediaQueue(mode).contains(media));

    kDebug() << "trying to play" << media.second;

    if (mode == MediaCenter::PictureMode) {
        setModeActive(MediaCenter::PictureMode, true);
        if (!m_picture->isVisible()) {
            QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
            m_video->hide();
            m_picture->show();
            layout->addItem(m_picture);
            layout->removeItem(m_videoProxy);
            setLayout(layout);
        }
        slideShow(media);
    } else if (mode == MediaCenter::MusicMode) {
        m_video->hide();
        setModeActive(MediaCenter::MusicMode, true);
        m_music->setCurrentSource(media.second);

        if (m_music->state() != Phonon::PlayingState) {
            setCurrentMedia(media, MediaCenter::MusicMode);
            emit newMedia(media);
            playPause(MediaCenter::MusicMode);
        }
    } else if (mode == MediaCenter::VideoMode) {
        setModeActive(MediaCenter::VideoMode, true);
        QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
        layout->removeItem(m_picture);
        layout->addItem(m_videoProxy);
        m_video->show();
        m_picture->hide();
        setLayout(layout);
        mediaObject(MediaCenter::VideoMode)->setCurrentSource(media.second);

        if (mediaObject(MediaCenter::VideoMode)->state() != Phonon::PlayingState) {
            setCurrentMedia(media, MediaCenter::VideoMode);
            emit newMedia(media);
            playPause(MediaCenter::VideoMode);
        }
    }
}

void MediaPlayer::slideShow(const MediaCenter::Media &media)
{
    kDebug() << "loading picture" << media.second;
    m_picture->loadPicture(media.second);
    setCurrentMedia(media, MediaCenter::PictureMode);

    if (slideShowInterval() == 0 && !m_picture->isTimerActive()) {
        setPlaybackState(MediaCenter::StoppedState, MediaCenter::PictureMode);
    }

    if (slideShowInterval() > 0 || m_picture->isTimerActive()) {
        if (playbackState(MediaCenter::PictureMode) == MediaCenter::PlayingState) {
            m_picture->startTimer();
            disconnect(m_picture, SIGNAL(showFinished()), this, SIGNAL(lastPictureShown()));
            disconnect(this, SIGNAL(lastPictureShown()), this, SLOT(returnToFirstPicture()));

            if (mediaQueue(MediaCenter::PictureMode).indexOf(media) == mediaQueue(MediaCenter::PictureMode).count() - 1) {
                //The picture state exits slideshow to return to the browser
                //after receiving this signal, also, if the user starts a new slideshow
                //it will start again from the first picture
                connect(m_picture, SIGNAL(showFinished()), this, SIGNAL(lastPictureShown()));
                connect(this, SIGNAL(lastPictureShown()), this, SLOT(returnToFirstPicture()));
            }
        }
    }
}

void MediaPlayer::setCurrentMode(MediaCenter::Mode mode)
{
    Player::setCurrentMode(mode);
    if (mode == MediaCenter::MusicMode) {
        m_video->hide();
        m_picture->hide();
    }
    if (mode == MediaCenter::VideoMode) {
        m_video->show();
        m_picture->hide();
    }
    if (mode == MediaCenter::PictureMode) {
        m_video->hide();
        m_picture->show();
    }
}

QRectF MediaPlayer::pictureRect() const
{
    return m_picture->pictureRect();
}

void MediaPlayer::returnToFirstPicture()
{
    setCurrentMedia(mediaQueue(MediaCenter::PictureMode).first(), MediaCenter::PictureMode); //For next slideshow to start at beginning
}

void MediaPlayer::slotEmitNewMedia()
{
    emit newMedia(currentMedia(MediaCenter::PictureMode));
}

void MediaPlayer::adjustActiveWindow(WId id)
{
    if (!m_video->isVisible()) {
        return;
    }

    if (id == m_video->winId()) {
        view()->activateWindow();
        view()->raise();
    }
}

void MediaPlayer::updateVideoWidgetGeometry()
{
    if (!m_video || m_videoProxy->widget() || !m_video->isVisible()) {
        return;
    }

    m_video->setGeometry(view()->geometry());
    m_video->move(view()->mapToGlobal(view()->pos()));
}

void MediaPlayer::setupForVideoWidget(bool restore)
{
    WId mainWindowId = m_mainWidget->winId();

    if (restore) {
        containment()->setWallpaper(m_previousWallpaperPlugin);

        view()->setPalette(QPalette());
        m_video->hide();
        Plasma::WindowEffects::overrideShadow(mainWindowId, false);
    } else {
        if (m_mainWidget->isFullScreen()) {
            m_videoProxy->setWidget(0);
            m_video->setParent(0);
            m_video->setWindowFlags(Qt::FramelessWindowHint);
            KWindowSystem::setState(m_video->winId(), NET::KeepAbove | NET::StaysOnTop);
            if (containment()->wallpaper()) {
                m_previousWallpaperPlugin = containment()->wallpaper()->name();
            }
            // we don't want any wallpaper plugin to be set
            containment()->setWallpaper("");
            QPalette p = view()->palette();
            p.setColor(QPalette::Base, Qt::transparent);
            view()->setPalette(p);
            Plasma::WindowEffects::overrideShadow(mainWindowId, true);
            m_video->show();
            updateVideoWidgetGeometry();
        } else {
            if (m_videoProxy->widget() != m_video) {
                m_videoProxy->setWidget(m_video);
            }
        }
    }
}

void MediaPlayer::reactToStateChange(MediaCenter::PlaybackState oldState, MediaCenter::PlaybackState state, MediaCenter::Mode mode)
{
    if (mode == MediaCenter::VideoMode) {
        if (state == MediaCenter::PlayingState && oldState != MediaCenter::PausedState) {
            setupForVideoWidget();
        } else if (state == MediaCenter::StoppedState) {
            setupForVideoWidget(true);
        }
    }
}

K_EXPORT_PLASMA_APPLET(mcplayer, MediaPlayer)

#include "mediaplayer.moc"
