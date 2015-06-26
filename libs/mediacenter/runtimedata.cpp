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

#include <KActivities/ResourceInstance>

RuntimeData::RuntimeData()
{
}

RuntimeData::~RuntimeData()
{
}

void RuntimeData::toggleMute() {
    if(m_mute && m_volume == 0) {
        m_volume = m_lastVolume;
        emit volumeChanged(m_volume);
    } else {
        m_volume = 0;
        emit volumeChanged(m_volume);
    }
    m_mute = !m_mute;
    emit muteChanged(m_mute);
}

void RuntimeData::playUrl(QUrl url) {
    m_url = url;
    m_status = PmcStatus::Playing;
    if(!m_testMode)
        KActivities::ResourceInstance::notifyAccessed(url);
    emit urlChanged(m_url);
    emit statusChanged(m_status);
}

void RuntimeData::playPause() {
    if (m_status == PmcStatus::Playing) {
        m_status = PmcStatus::Paused;
        emit statusChanged(m_status);
    } else {
        m_status = PmcStatus::Playing;
        emit statusChanged(m_status);
    }
}

void RuntimeData::stop() {
    if(m_status != PmcStatus::Stopped) {
        m_status = PmcStatus::Stopped;
        emit statusChanged(m_status);
    }
}

void RuntimeData::setVolume(qreal volume) {
    if (m_mute && volume > 0) {
        m_mute = false;
        m_lastVolume = m_volume;
        emit muteChanged(m_mute);
    }

    if (!m_mute && volume == 0) {
        m_mute = true;
        m_lastVolume = m_volume;
        emit muteChanged(m_mute);
    }

    m_volume = volume;
    emit volumeChanged(m_volume);
}

void RuntimeData::setTestMode(bool mode) {
    if(m_testMode != mode)
    {
        m_testMode = mode;
        emit testModeChanged(m_testMode);
    }
}
