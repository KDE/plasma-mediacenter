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
#include <QTimer>
#include <QDebug>

#define UPDATE(property, value) \
    if (property != value) { \
        property = value; \
        emitUpdate(); \
        return true; \
    } \
    return false;

class Media::Private
{
public:
    //This timer is used to emit the updated() signal only once after more than
    //one property has been updateds
    QTimer updateTimer;
};

Media::Media(const QString& url, QObject* parent)
    : QObject(parent)
    , m_sha(calculateSha(url))
    , m_url(url)
    , d(new Private())
{
    initUpdateTimer();
}

Media::Media(QObject* parent): QObject(parent), d(new Private())
{
    initUpdateTimer();
}

void Media::initUpdateTimer()
{
    d->updateTimer.setInterval(0);
    d->updateTimer.setSingleShot(true);
    connect(&d->updateTimer, SIGNAL(timeout()), SIGNAL(updated()));
}

void Media::emitUpdate()
{
    d->updateTimer.start();
}

QString Media::calculateSha(const QString& url)
{
    QCryptographicHash hash(QCryptographicHash::Sha1);
    hash.addData(url.toUtf8());
    return hash.result().toHex();
}

const QString& Media::title() const
{
    return m_title;
}

bool Media::setTitle(const QString& title)
{
    UPDATE(m_title, title);
}

const QString& Media::url() const
{
    return m_url;
}

const QString& Media::thumbnail() const
{
    return m_thumbnail;
}

bool Media::setThumbnail(const QString& thumbnail)
{
    UPDATE(m_thumbnail, thumbnail);
}

const QString& Media::sha() const
{
    return m_sha;
}

bool Media::setType(const QString& type)
{
    UPDATE(m_type, type);
}

const QString& Media::type() const
{
    return m_type;
}

bool Media::setValueForRole(int role, const QVariant& value)
{
    switch (role) {
        case MediaCenter::MediaTypeRole:
            return setType(value.toString());
        case Qt::DisplayRole:
            return setTitle(value.toString());
        case MediaCenter::MediaUrlRole:
            if (m_url != value.toString()) {
                qWarning() << "Media URLs CANNOT be changed";
            }
            return false;
        case MediaCenter::MediaThumbnailRole:
            return setThumbnail(value.toString());
        case Qt::DecorationRole:
            return thumbnail().isEmpty() ? setThumbnail(value.toString()) : false;
        default:
            //qWarning() << "Unknown role " << role << " for value " << value;
            return false;
    }
}
