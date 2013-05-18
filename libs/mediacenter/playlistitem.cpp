/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                              *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "playlistitem.h"

#include <fileref.h>

#include <KLocalizedString>

#include <QtCore/QTimer>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

#include <QDebug>

PlaylistItem::PlaylistItem(const QString& url, QObject* parent)
    : QObject(parent)
    , m_mediaUrl(url)
    , m_mediaLength(-1)
{
    m_updateTimer.setInterval(0);
    m_updateTimer.setSingleShot(true);
    connect(&m_updateTimer, SIGNAL(timeout()), SLOT(update()));
}

QString PlaylistItem::mediaUrl() const
{
    return m_mediaUrl;
}

QString PlaylistItem::mediaName() const
{
    if (m_mediaName.isEmpty()) {
        m_updateTimer.start();
    }
    return m_mediaName;
}

QString PlaylistItem::mediaArtist() const
{
    if (m_mediaArtist.isEmpty()) {
        m_updateTimer.start();
    }
    return m_mediaArtist;
}

int PlaylistItem::mediaLength() const
{
    if (m_mediaLength == -1) {
        m_updateTimer.start();
    }
    return m_mediaLength;
}

void PlaylistItem::update()
{
    TagLib::FileRef f(QUrl(m_mediaUrl).toLocalFile().toUtf8().constData());

    if(!f.isNull() && f.tag()) {
        m_mediaName = f.tag()->title().toCString(true);
        m_mediaArtist = f.tag()->artist().toCString(true);
        if (m_mediaName.isEmpty()) {
            m_mediaName = QFileInfo(m_mediaUrl).fileName();
        }
    } else {
        m_mediaName = QFileInfo(m_mediaUrl).fileName();
    }

    if (!f.isNull() && f.audioProperties()) {
        m_mediaLength = f.audioProperties()->length();
    } else {
        m_mediaLength = 0;
    }

    emit updated();
}

#include "playlistitem.moc"
