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

PlaylistItem::PlaylistItem(const QString& url, QObject* parent)
    : QObject(parent)
{
    m_media = SingletonFactory::instanceFor<MediaLibrary>()->mediaForUrl(url);
    connect(m_media.data(), SIGNAL(updated()), SIGNAL(updated()));
}

QString PlaylistItem::mediaUrl() const
{
    return m_media->url();
}

QString PlaylistItem::mediaName() const
{
    return m_media->title();
}

QString PlaylistItem::mediaArtist() const
{
    return m_media->artist();
}

int PlaylistItem::mediaLength() const
{
    return m_media->duration();
}
