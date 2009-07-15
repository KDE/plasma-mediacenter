/***************************************************************************
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>                    *
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
//#include "plasmamediaplayeradaptor.h"
//#include "dbus/playerdbushandler.h"
//#include "dbus/rootdbushandler.h"
//#include "dbus/tracklistdbushandler.h"

#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneDragDropEvent>
#include <QDBusConnection>
#include <QTimer>

#include <KMimeType>
#include <KFileDialog>
#include <KConfigDialog>

#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>

#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/slider.h>
#include <plasma/widgets/videowidget.h>

MediaPlayer::MediaPlayer(QObject *parent, const QVariantList &args)
    : MediaCenter::Player(parent, args),
      m_ticking(false),
      m_raised(false),
      m_fullScreen(false),
      m_fullScreenVideo(0)
{
    setAcceptDrops(true);
    setHasConfigurationInterface(true);
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    resize(200, 200);

    if (args.count()) {
        m_currentUrl = args.value(0).toString();
    }

//    QDBusConnection dbus = QDBusConnection::sessionBus();
//    dbus.registerService("org.mpris.PlasmaMediaPlayer");
//    new PlasmaMediaPlayerAdaptor(this);
//
//    dbus.registerObject("/PlasmaMediaPlayer", this);
}


MediaPlayer::~MediaPlayer()
{
    delete m_video;
}

void MediaPlayer::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *widget = new QWidget(parent);
    ui.setupUi(widget);

    parent->addPage(widget, i18n("Configure"), "configure");

    ui.fullScreenCheckBox->setChecked(m_fullScreen);
    connect (parent, SIGNAL(accepted()), this, SLOT(acceptConfiguration()));
}

void MediaPlayer::acceptConfiguration()
{
    m_fullScreen = ui.fullScreenCheckBox->isChecked();
    applyConfig();
}

void MediaPlayer::applyConfig()
{
    doFullScreen();
}

void MediaPlayer::doFullScreen()
{
    if (m_fullScreen) {
        m_fullScreenVideo = m_video->nativeWidget();
        m_video->setWidget(0);
        m_fullScreenVideo->setParent(0);
        m_fullScreenVideo->installEventFilter(this);
        m_fullScreenVideo->enterFullScreen();
    } else {
        m_fullScreenVideo->exitFullScreen();
        m_fullScreenVideo->removeEventFilter(this);
        m_video->setWidget(m_fullScreenVideo);
    }


}

void MediaPlayer::init()
{
   m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);

   m_video = new Plasma::VideoWidget(this);
   m_video->setAcceptDrops(false);

   m_layout->addItem(m_video);



//   connect(m_video->audioOutput(), SIGNAL(volumeChanged(qreal)), SLOT(volumeChanged(qreal)));


   m_video->setUrl(m_currentUrl);
   Phonon::MediaObject *media = m_video->mediaObject();

//   connect(media, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(stateChanged(Phonon::State, Phonon::State)));
//   connect(media, SIGNAL(seekableChanged(bool)), this, SLOT(seekableChanged(bool)));

   media->setTickInterval(200);

//   connect(media, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
//   connect(media, SIGNAL(totalTimeChanged(qint64)), SLOT(totalTimeChanged(qint64)));

   media->play();

//   m_video->setUsedControls(Plasma::VideoWidget::DefaultControls);

   m_hideTimer = new QTimer(this);
   m_hideTimer->setSingleShot(true);
   connect(m_hideTimer, SIGNAL(timeout()), this, SLOT(hideControls()));

   SetControlsVisible(false);

//   new PlayerDBusHandler(this, media, m_video->audioOutput());
//   new TrackListDBusHandler(this, media);
//   new RootDBusHandler(this);
}



void MediaPlayer::constraintsEvent(Plasma::Constraints constraints)
{
}

void MediaPlayer::SetControlsVisible(bool visible)
{
    m_video->setControlsVisible(visible);
}

bool MediaPlayer::ControlsVisible() const
{
    return m_video->controlsVisible();
}

void MediaPlayer::ToggleControlsVisibility()
{
    SetControlsVisible(!m_video->controlsVisible());
}

void MediaPlayer::playPause()
{
    Phonon::MediaObject *media = m_video->mediaObject();

    if (media->state() == Phonon::PlayingState) {
        media->pause();
    } else {
        media->play();
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

void MediaPlayer::seek(int progress)
{
    if (!m_ticking) {
        m_video->mediaObject()->seek(progress);
    }
}

void MediaPlayer::setVolume(qreal value)
{
     m_video->audioOutput()->setVolume(value);
}

void MediaPlayer::dropEvent(QGraphicsSceneDragDropEvent *event)
{

    QString text = event->mimeData()->text();

    QUrl testPath(text);

    if (QFile::exists(testPath.path())) {
        KMimeType::Ptr type = KMimeType::findByPath(testPath.path());

        if (type->name().indexOf("video/") != -1) {
            OpenUrl(testPath.path());
        }
    }
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
    Phonon::MediaObject *media = m_video->mediaObject();
    Phonon::AudioOutput *audio = m_video->audioOutput();
    const qreal step = 30;

    switch (event->key())
    {
    case Qt::Key_Left:
        media->seek(media->currentTime() - media->totalTime()/step);
        break;
    case Qt::Key_Right:
        media->seek(media->currentTime() + media->totalTime()/step);
        break;
    case Qt::Key_Space:
        playPause();
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
        m_hideTimer->start(2000);
    }
}

bool MediaPlayer::eventFilter(QObject *o, QEvent *e)
{
    if (o != m_fullScreenVideo) {
        return false;
    }

    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(e);
        if (keyEvent->key() == Qt::Key_Escape) {
            m_fullScreen = false;
            doFullScreen();
            return true;
        }
    }

    return false;
}

void MediaPlayer::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   event->accept();
}

void MediaPlayer::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
    ToggleControlsVisibility();
}

void MediaPlayer::stop()
{
    m_video->mediaObject()->stop();
}

Phonon::MediaObject* MediaPlayer::mediaObject()
{
    if (m_video) {
        return m_video->mediaObject();
    }
    return 0;
}

void MediaPlayer::skipBackward()
{}

void MediaPlayer::skipForward()
{}

K_EXPORT_PLASMA_APPLET(mcplayer, MediaPlayer)

#include "mediaplayer.moc"
