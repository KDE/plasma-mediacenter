/***************************************************************************
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
#include "player.h"

Player::Player(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args)
{
}

Player::~Player()
{}

QString Player::currentMedia()
{}

void Player::setQueue(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::enqueue(const QList<Media> &sources)
{
    Q_UNUSED(sources);
}

void Player::enqueue(int index, const QList<Media> &sources)
{
    Q_UNUSED(index);
    Q_UNUSED(sources);
}

void Player::setSlideshowInterval(qint64 time)
{
    Q_UNUSED(time);
}
