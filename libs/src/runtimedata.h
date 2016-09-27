/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RUNTIMEDATA_H
#define RUNTIMEDATA_H

#include <QObject>
#include <QUrl>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT RuntimeData : public QObject
{
    Q_OBJECT
    Q_ENUMS(PmcStatus)

    Q_PROPERTY(bool userTrigerredStop MEMBER m_userTrigerredStop NOTIFY userTrigerredStopChanged)
    Q_PROPERTY(qreal volume MEMBER m_volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(int currentTime MEMBER m_currentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(int totalTime MEMBER m_totalTime NOTIFY totalTimeChanged)
    Q_PROPERTY(int position MEMBER m_position NOTIFY positionChanged)
    Q_PROPERTY(bool mute MEMBER m_mute NOTIFY muteChanged)
    Q_PROPERTY(QUrl url MEMBER m_url NOTIFY urlChanged)
    Q_PROPERTY(PmcStatus status MEMBER m_status NOTIFY statusChanged)
    Q_PROPERTY(bool testMode MEMBER m_testMode WRITE setTestMode NOTIFY testModeChanged)

public:
    RuntimeData();
    ~RuntimeData();

    Q_INVOKABLE void toggleMute();
    Q_INVOKABLE void playUrl(QUrl url);
    Q_INVOKABLE void playPause();
    Q_INVOKABLE void stop();

    enum PmcStatus {
        Playing,
        Stopped,
        Paused
    };

    void setVolume(qreal volume);
    void setTestMode(bool mode);

Q_SIGNALS:
    void userTrigerredStopChanged();
    void volumeChanged(qreal volume);
    void currentTimeChanged(int currentTime);
    void totalTimeChanged(int totalTime);
    void positionChanged(int position);
    void statusChanged(PmcStatus status);
    void muteChanged(bool muted);
    void urlChanged(QUrl url);
    void testModeChanged(bool mode);

private:
    bool m_userTrigerredStop;
    qreal m_volume = 0.7;
    int m_currentTime;
    int m_totalTime;
    int m_position;
    QUrl m_url;
    bool m_mute;
    qreal m_lastVolume;
    PmcStatus m_status;
    bool m_testMode = false;
};

#endif // RUNTIMEDATA_H
