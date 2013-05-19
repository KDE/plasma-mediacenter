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
#include "mediainforequest.h"
#include "mediainfoservice.h"

#include <QtCore/QTimer>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

#include <QDebug>

PlaylistItem::PlaylistItem(const QString& url, QObject* parent)
    : QObject(parent)
    , m_mediaUrl(url)
    , m_mediaLength(-1)
{
    m_updateTimer.setInterval(1000);
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
        return QFileInfo(m_mediaUrl).fileName();
    }
    return m_mediaName;
}

QString PlaylistItem::mediaArtist() const
{
    if (m_mediaArtist.isEmpty()) {
        m_updateTimer.start();
        return "--";
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
    MediaInfoRequest *request = new MediaInfoRequest(m_mediaUrl);
    request->addRequest(MediaInfoRequest::Title)
           ->addRequest(MediaInfoRequest::Artist)
           ->addRequest(MediaInfoRequest::Length);

    MediaInfoService *service = MediaInfoService::instance();
    connect(service, SIGNAL(info(quint64,MediaInfoResult)), SLOT(processMediaInfo(quint64,MediaInfoResult)));
    m_serviceRequestNumber = service->processRequest(request);
}

void PlaylistItem::processMediaInfo(quint64 requestNumber, MediaInfoResult info)
{
    disconnect(sender());
    if (requestNumber != m_serviceRequestNumber)
        return;

    m_mediaName = info.data(MediaInfoRequest::Title).toString();
    m_mediaArtist = info.data(MediaInfoRequest::Artist).toString();
    m_mediaLength = info.data(MediaInfoRequest::Length).toInt();

    emit updated();
}

#include "playlistitem.moc"
