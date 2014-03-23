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
#include "singletonfactory.h"
#include "medialibrary.h"
#include "pmcmedia.h"

const char *PlaylistItem::defaultArtist = "--";
const int PlaylistItem::defaultLength = -1;

void PlaylistItem::init(const QString& url)
{
    m_media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(url);
    m_mediaUrl = url;
}

PlaylistItem::PlaylistItem(const QString& url, QObject* parent)
    : QObject(parent)
    , m_mediaLength(defaultLength)
{
    init(url);
}

PlaylistItem::PlaylistItem(const QString& url, const QString& name,
                           const QString& artist, int length, QObject* parent)
    : QObject(parent)
{
    init(url);

    m_mediaName = name;
    m_mediaArtist = artist;
    m_mediaLength = length;

}

QString PlaylistItem::mediaUrl() const
{
    return m_mediaUrl;
}

QString PlaylistItem::mediaName() const
{
    return !m_media.isNull() ? m_media->title() : m_mediaName;
}

QString PlaylistItem::mediaArtist() const
{
    return !m_media.isNull() ? m_media->artist() : m_mediaArtist;
}

int PlaylistItem::mediaLength() const
{
    return !m_media.isNull() ? m_media->duration() : m_mediaLength;
}

#include "playlistitem.moc"
