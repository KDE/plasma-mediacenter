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
#include "mediacenter.h"

#include <QCryptographicHash>
#include <QTimer>

#include <QxMemLeak.h>

QX_REGISTER_CPP_PMC(Media)

namespace qx {
template <> void register_class(QxClass<Media> & m)
{
    m.id(& Media::m_sha, "sha");

    m.data(& Media::m_title, "title");
    m.data(& Media::m_url, "url");
    m.data(& Media::m_thumbnail, "thumbnail");
    m.data(& Media::m_type, "type");
    m.data(& Media::m_duration, "duration");

    m.relationManyToOne(& Media::m_album, "album_id");
    m.relationManyToOne(& Media::m_artist, "artist_id");
}}

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

Media::~Media()
{
    delete d;
}

template <class T>
bool Media::updateIfChanged(T& variable, const T& newValue)
{
    if (variable != newValue) {
        variable = newValue;
        emitUpdate();
        return true;
    }
    return false;
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
    return updateIfChanged(m_title, title);
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
    return updateIfChanged(m_thumbnail, thumbnail);
}

const QString& Media::sha() const
{
    return m_sha;
}

bool Media::setType(const QString& type)
{
    return updateIfChanged(m_type, type);
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
        case Qt::DecorationRole:
            return thumbnail().isEmpty() ? setThumbnail(value.toString()) : false;
        default:
            //qWarning() << "Unknown role " << role << " for value " << value;
            return false;
    }
}

const QSharedPointer< Album >& Media::album() const
{
    return m_album;
}

void Media::setAlbum(const QSharedPointer< Album >& album)
{
    updateIfChanged(m_album, album);
}

const QSharedPointer< Artist >& Media::artist() const
{
    return m_artist;
}

void Media::setArtist(const QSharedPointer< Artist >& artist)
{
    updateIfChanged(m_artist, artist);
}

int Media::duration() const
{
    return m_duration;
}

bool Media::setDuration(int duration)
{
    return updateIfChanged(m_duration, duration);
}
