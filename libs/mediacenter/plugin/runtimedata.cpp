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

void RuntimeData::toggleMute() {
   if(m_volume == 0) {
       m_volume = m_lastVolume;
       emit volumeChanged(m_volume);
   } else {
       m_lastVolume = m_volume;
       m_volume = 0.0;
       emit volumeChanged(m_volume);
   }
}

void RuntimeData::playUrl(QUrl url) {
    m_url = url;
    emit urlChanged(m_url);
}
#include "runtimedata.moc"
