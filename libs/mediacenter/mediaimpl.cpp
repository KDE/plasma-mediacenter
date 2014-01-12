/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#include "media.h"

#include <QCryptographicHash>

MediaImpl::MediaImpl (const QString& type, const QString& title, const QString& url,
              const QString& thumbnail)
    : m_type(type), m_title(title), m_url(url), m_thumbnail(thumbnail)
{
    m_sha = calculateSha(url);
}

MediaImpl::MediaImpl ()
{
}

QString MediaImpl::calculateSha(const QString& url)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(url.toUtf8());
    return hash.result().toHex();
}

const QString& MediaImpl::title() const
{
    return m_title;
}

void MediaImpl::setTitle(const QString& title)
{
    m_title = title;
}

const QString& MediaImpl::url() const
{
    return m_url;
}

void MediaImpl::setUrl(const QString& url)
{
    m_url = url;
}

const QString& MediaImpl::thumbnail() const
{
    return m_thumbnail;
}

void MediaImpl::setThumbnail(const QString& thumbnail)
{
    m_thumbnail = thumbnail;
}

const QString& MediaImpl::sha() const
{
    return m_sha;
}

void MediaImpl::setType(const QString& type)
{
    m_type = type;
}

const QString& MediaImpl::type() const
{
    return m_type;
}
