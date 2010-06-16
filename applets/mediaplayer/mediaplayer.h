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

    Q_PROPERTY(bool ControlsVisible READ ControlsVisible WRITE SetControlsVisible)

public:
    MediaPlayer(QObject *parent, const QVariantList &args);
    ~MediaPlayer();

    void init();
    void constraintsEvent(Plasma::Constraints constraints);
    void SetControlsVisible(bool visible);
    bool ControlsVisible() const;
    void createConfigurationInterface(KConfigDialog *parent);

    virtual Phonon::MediaObject* mediaObject();

    bool eventFilter(QObject *o, QEvent *e);

    virtual void setPlayerType(const MediaCenter::MediaType &type);

    virtual MediaCenter::PlaybackState videoPlayerPlaybackState() const;
    virtual MediaCenter::PlaybackState musicPlayerPlaybackState() const;
    virtual MediaCenter::PlaybackState picturePlayerPlaybackState() const;

    virtual QRectF pictureRect() const;

public slots:
    virtual void playPauseVideo();
    virtual void playPauseMusic();
    virtual void playPausePicture();
    void ToggleControlsVisibility();
    void RaiseLower();
    void Raise();
    void Lower();
    virtual void seekVideo(const int progress);
    virtual void seekMusic(const int progress);
    virtual void setVolume(const int value);
    virtual qreal volume() const;
    void ShowOpenFileDialog();
    void OpenUrl(const QString &path);

    virtual void playVideoMedia(const MediaCenter::Media &media);
    virtual void playAllVideoMedia();
    virtual void playPictureMedia(const MediaCenter::Media &media);
    virtual void playAllPictureMedia();
    virtual void playMusicMedia(const MediaCenter::Media &media);
    virtual void playAllMusicMedia();
    virtual void enqueueVideos(const QList<MediaCenter::Media> &sources);
    virtual void clearVideoQueue();
    virtual void enqueuePictures(const QList<MediaCenter::Media> &sources);
    virtual void clearPictureQueue();
    virtual void enqueueMusic(const QList<MediaCenter::Media> &sources);
    virtual void clearMusicQueue();

    virtual void stopVideo();
    virtual void skipVideoForward();
    virtual void skipVideoBackward();
    virtual void stopMusic();
    virtual void skipMusicForward();
    virtual void skipMusicBackward();
    virtual void stopPicture();
    virtual void skipPictureForward();
    virtual void skipPictureBackward();

    void slideShow(const MediaCenter::Media &);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void keyPressEvent(QKeyEvent *event);

private slots:
    void hideControls();
    void acceptConfiguration();
    void playNextPictureMedia();
    void playNextMusicMedia();
    void playNextVideoMedia();
    void returnToFirstPicture();
    void slotEmitNewMedia();

private:
    void loadConfig();
    void applyConfig();

    /**
     * Appends a media file to the m_medias list
     * filling also the MediaCenter::MediaType.
     */
    void appendMusic(const QStringList &medias);
    void appendPicture(const QStringList &medias);
    void appendVideo(const QStringList &medias);

private:
    QGraphicsLinearLayout *m_layout;
    QString m_currentUrl;
    Plasma::VideoWidget *m_video;
    Plasma::VideoWidget *m_music;
    QTimer *m_hideTimer;

    bool m_ticking;
    bool m_raised;

    bool m_fullScreen;
    qint64 m_sshowTime;
    PictureViewer *m_picture;

    MediaCenter::PlaybackState m_currentVideoPlaybackState;
    MediaCenter::PlaybackState m_currentPicturePlaybackState;
    MediaCenter::PlaybackState m_currentMusicPlaybackState;

    MediaCenter::MediaType m_currentStateType;

    Ui::Config ui;

    QList<MediaCenter::Media> m_videoMedias;
    QList<MediaCenter::Media> m_pictureMedias;
    QList<MediaCenter::Media> m_musicMedias;
};

#endif
