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

#ifndef MEDIAPLAYER_HEADER
#define MEDIAPLAYER_HEADER

#include <mediacenter/player.h>

#include "ui_config.h"

#include <phonon/mediaobject.h>

class QGraphicsSceneDragDropEvent;
class QGraphicsLinearLayout;
class QTimer;
class PictureViewer;

namespace Phonon
{
    class VideoWidget;
}

namespace Plasma
{
    class VideoWidget;
}

class KConfigDialog;


class MediaPlayer : public MediaCenter::Player
{
    Q_OBJECT

public:
    MediaPlayer(QObject *parent, const QVariantList &args);
    ~MediaPlayer();

    void init();
    void constraintsEvent(Plasma::Constraints constraints);
    void createConfigurationInterface(KConfigDialog *parent);

    bool eventFilter(QObject *o, QEvent *e);

    void setCurrentMode(MediaCenter::Mode mode);

    QRectF pictureRect() const;

public slots:
    void playPause(MediaCenter::Mode);

    void RaiseLower();
    void Raise();
    void Lower();

    void stop(MediaCenter::Mode);

    void skipForward(MediaCenter::Mode);
    void skipBackward(MediaCenter::Mode);

    void slideShow(const MediaCenter::Media &);

    void playMedia(MediaCenter::Mode, const MediaCenter::Media &media);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void acceptConfiguration();
    void returnToFirstPicture();
    void slotEmitNewMedia();
    void playNextMedia(MediaCenter::Mode);
    void playNextVideoMedia();
    void playNextPictureMedia();
    void playNextMusicMedia();
    void adjustActiveWindow(WId id);
    void reactToStateChange(MediaCenter::PlaybackState, MediaCenter::PlaybackState, MediaCenter::Mode);

private:
    void loadConfig();
    void applyConfig();

    void updateVideoWidgetGeometry();
    void setupForVideoWidget(bool restore = false);

private:
    QGraphicsLinearLayout *m_layout;
    QString m_currentUrl;
    Phonon::VideoWidget *m_video;
    Phonon::MediaObject *m_music;
    QTimer *m_hideTimer;
    QString m_previousWallpaperPlugin;

    bool m_raised;

    bool m_fullScreen;
    qint64 m_sshowTime;
    PictureViewer *m_picture;

    QWidget *m_mainWidget;
    QGraphicsProxyWidget *m_videoProxy;

    Ui::Config ui;
};

#endif
