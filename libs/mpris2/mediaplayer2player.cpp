/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
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

#include "mpris2/mediaplayer2player.h"

#include <QCryptographicHash>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QVariant>
#include <QFile>

#include <KUrl>
#include <KStandardDirs>

MediaPlayer2Player::MediaPlayer2Player(QObject* parent)
    : QDBusAbstractAdaptor(parent)
{
}

MediaPlayer2Player::~MediaPlayer2Player()
{
}

bool MediaPlayer2Player::CanGoNext() const
{
    return true;
}

void MediaPlayer2Player::Next() const
{
}

bool MediaPlayer2Player::CanGoPrevious() const
{
    return true;
}

void MediaPlayer2Player::Previous() const
{
}

bool MediaPlayer2Player::CanPause() const
{
    return true;
}

void MediaPlayer2Player::Pause() const
{
}

void MediaPlayer2Player::PlayPause() const
{
}

void MediaPlayer2Player::Stop() const
{
}

bool MediaPlayer2Player::CanPlay() const
{
    return true;
}

void MediaPlayer2Player::Play() const
{
}

bool MediaPlayer2Player::Shuffle() const
{
    // TODO: Implement
    return false;
}

void MediaPlayer2Player::setShuffle(bool shuffle) const
{
    Q_UNUSED(shuffle)
    // TODO: Implement
}

double MediaPlayer2Player::Volume() const
{
    return 1.0;
}

void MediaPlayer2Player::setVolume(double volume) const
{
    if (volume < 0.0)
        volume = 0.0;
    if (volume > 1.0)
        volume = 1.0;
}

qlonglong MediaPlayer2Player::Position() const
{
    return 2 * 1000;
}

bool MediaPlayer2Player::CanSeek() const
{
    return true;
}

bool MediaPlayer2Player::CanControl() const
{
    return true;
}

void MediaPlayer2Player::volumeChanged(float newVol) const
{
    Q_UNUSED(newVol)

    QVariantMap properties;
    properties["Volume"] = Volume();
    signalPropertiesChange(properties);
}

void MediaPlayer2Player::seeked(int newPos) const
{
    // casts int to uint64
    emit Seeked(newPos);
}

void MediaPlayer2Player::signalPropertiesChange(const QVariantMap& properties) const
{
    QDBusMessage msg = QDBusMessage::createSignal("/org/mpris/MediaPlayer2",
        "org.freedesktop.DBus.Properties", "PropertiesChanged" );

    QVariantList args;
    args << "org.mpris.MediaPlayer2.Player";
    args << properties;
    args << QStringList();

    msg.setArguments(args);

    QDBusConnection::sessionBus().send(msg);
}
