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

#include "mpris2/mediaplayer2.h"

#include <QDBusConnection>
#include <QDBusMessage>

#include <KAboutData>
#include <KApplication>
#include <KCmdLineArgs>

MediaPlayer2::MediaPlayer2(QObject* parent) : QDBusAbstractAdaptor(parent)
{
    QDBusConnection::sessionBus().registerObject("/org/mpris/MediaPlayer2", this, QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties | QDBusConnection::ExportAllSignals);
}

MediaPlayer2::~MediaPlayer2()
{
}

bool MediaPlayer2::CanQuit() const
{
    return true;
}

bool MediaPlayer2::CanSetFullscreen() const
{
    return true;
}

bool MediaPlayer2::Fullscreen() const
{
    return true;
}

QString MediaPlayer2::Identity() const
{
    return KCmdLineArgs::aboutData()->programName();
}
