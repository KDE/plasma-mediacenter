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

#include "runtimedata.h"

RuntimeData::RuntimeData()
{
}

RuntimeData::~RuntimeData()
{
}

bool RuntimeData::userTrigerredStop() const
{
    return m_userTrigerredStop;
}

void RuntimeData::setUserTrigerredStop(bool userTrigerredStop)
{
    m_userTrigerredStop = userTrigerredStop;
}

qreal RuntimeData::volume() const
{
    return m_volume;
}

void RuntimeData::setVolume(qreal volume)
{
    m_volume = volume;
}

int RuntimeData::currentTime() const
{
    return m_currentTime;
}

void RuntimeData::setCurrentTime(int currentTime)
{
    m_currentTime = currentTime;
}

int RuntimeData::totalTime() const
{
    return m_totalTime;
}

void RuntimeData::setTotalTime(int totalTime)
{
    m_totalTime = totalTime;
}

int RuntimeData::position() const
{
    return m_position;
}

void RuntimeData::setPosition(int position)
{
    m_position = position;
}

RuntimeData::PmcStatus RuntimeData::status() const
{
    return m_status;
}

void RuntimeData::setStatus(PmcStatus status)
{
    m_status = status;
}

#include "runtimedata.moc"
