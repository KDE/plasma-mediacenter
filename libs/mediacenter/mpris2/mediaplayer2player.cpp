/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
 *   Copyright 2014 Ashish Madeti <ashishmadeti@gmail.com>                 *
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

#include "mediaplayer2player.h"
#include "mpris2.h"

#include <QCryptographicHash>
#include <QStringList>
#include <QMetaClassInfo>
#include <QDBusMessage>
#include <QDBusConnection>

static const double MAX_RATE = 32.0;
static const double MIN_RATE = 0.0;

MediaPlayer2Player::MediaPlayer2Player(QObject* parent)
    : QDBusAbstractAdaptor(parent),
      m_rate(0),
      m_paused(false),
      m_stopped(true)
{
}

MediaPlayer2Player::~MediaPlayer2Player()
{
}

QString MediaPlayer2Player::PlaybackStatus() const
{
    if (stopped()) {
        return QLatin1String("Stopped");
    } else if (paused()) {
        return QLatin1String("Paused");
    } else
        return QLatin1String("Playing");
}

bool MediaPlayer2Player::CanGoNext() const
{
    return mediaPlayerPresent();
}

void MediaPlayer2Player::Next() const
{
    emit next();
}

bool MediaPlayer2Player::CanGoPrevious() const
{
    return mediaPlayerPresent();
}

void MediaPlayer2Player::Previous() const
{
    emit previous();
}

bool MediaPlayer2Player::CanPause() const
{
    return mediaPlayerPresent();
}

void MediaPlayer2Player::Pause() const
{
    emit pause();
}

void MediaPlayer2Player::PlayPause()
{
    emit playPause();
}

int MediaPlayer2Player::stopped() const
{
    return m_stopped;
}

void MediaPlayer2Player::setStopped(int newVal)
{
    if (mediaPlayerPresent()) {
        m_stopped = newVal;

        signalPropertiesChange("PlaybackStatus", PlaybackStatus());
    }
}

int MediaPlayer2Player::paused() const
{
    return m_paused;
}

void MediaPlayer2Player::setPaused(int newVal)
{
    if (mediaPlayerPresent()) {
        m_paused = newVal;

        signalPropertiesChange("PlaybackStatus", PlaybackStatus());
    }
}

void MediaPlayer2Player::Stop() const
{
    emit stop();
}

bool MediaPlayer2Player::CanPlay() const
{
    return mediaPlayerPresent();
}

void MediaPlayer2Player::Play() const
{
    emit play();
}

double MediaPlayer2Player::Volume() const
{
    return m_volume;
}

void MediaPlayer2Player::setVolume(double volume)
{
    m_volume= qBound(0.0, volume, 1.0);
    emit volumeChanged(m_volume);

    signalPropertiesChange("Volume", Volume());
}

QVariantMap MediaPlayer2Player::Metadata() const
{
    return m_metadata;
}

qlonglong MediaPlayer2Player::Position() const
{
    return m_position;
}

void MediaPlayer2Player::setPropertyPosition(int newPositionInMs)
{
    m_position = qlonglong(newPositionInMs)*1000;
    //PMC stores postion in milli-seconds, Mpris likes it in micro-seconds
}

double MediaPlayer2Player::Rate() const
{
    return m_rate;
}

void MediaPlayer2Player::setRate(double newRate)
{
    if (newRate == 0) {
        Pause();
    } else {
        m_rate = qBound(MinimumRate(), newRate, MaximumRate());
        emit rateChanged(m_rate);

        signalPropertiesChange("Rate", Rate());
    }
}

double MediaPlayer2Player::MinimumRate() const
{
    return MIN_RATE;
}

double MediaPlayer2Player::MaximumRate() const
{
    return MAX_RATE;
}

bool MediaPlayer2Player::CanSeek() const
{
    return mediaPlayerPresent();
}

bool MediaPlayer2Player::CanControl() const
{
    return true;
}

void MediaPlayer2Player::Seek(qlonglong Offset) const
{
    if (mediaPlayerPresent()) {
        //The seekBy function (to which this signal is linked to) accepts offset in seconds
        int offset = Offset/1000000;
        emit seek(offset);
    }
}

void MediaPlayer2Player::emitSeeked(int pos)
{
    emit Seeked(qlonglong(pos)*1000);
}

void MediaPlayer2Player::SetPosition(const QDBusObjectPath& trackId, qlonglong pos)
{
    if (trackId.path() == static_cast<Mpris2*>(parent())->getCurrentTrackId())
        seek((pos - Position())/1000000);
}

void MediaPlayer2Player::OpenUri(QString uri) const
{
    QUrl url(uri);
    if (url.isLocalFile()) {
        emit playUrl(uri);
    }
}

QUrl MediaPlayer2Player::currentTrack() const
{
    return m_currentTrack;
}

void MediaPlayer2Player::setCurrentTrack(QUrl newTrack)
{
    m_currentTrack = newTrack;
    m_metadata = static_cast<Mpris2*>(parent())->getMetadataOf(m_currentTrack.toString());

    signalPropertiesChange("Metadata", Metadata());
}

int MediaPlayer2Player::mediaPlayerPresent() const
{
    return m_mediaPlayerPresent;
}

void MediaPlayer2Player::setMediaPlayerPresent(int status)
{
    if (m_mediaPlayerPresent != status) {
        m_mediaPlayerPresent = status;

        signalPropertiesChange("CanGoNext", CanGoNext());
        signalPropertiesChange("CanGoPrevious", CanGoPrevious());
        signalPropertiesChange("CanPause", CanPause());
        signalPropertiesChange("CanPlay", CanPlay());
        signalPropertiesChange("CanSeek", CanSeek());
    }
}

void MediaPlayer2Player::signalPropertiesChange(const QString &property, const QVariant &value)
{
    QVariantMap properties;
    properties[property] = value;
    const int ifaceIndex = metaObject()->indexOfClassInfo("D-Bus Interface");
    QDBusMessage msg = QDBusMessage::createSignal("/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties", "PropertiesChanged");

    msg << metaObject()->classInfo(ifaceIndex).value();
    msg << properties;
    msg << QStringList();

    QDBusConnection::sessionBus().send(msg);
}
