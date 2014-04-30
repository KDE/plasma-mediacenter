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

#include "album.h"
#include "artist.h"

#include "mediacenter.h"

#include <QCryptographicHash>
#include <QTimer>

#include <QDebug>

class Media::Private
{
public:
    //This timer is used to emit the updated() signal only once after more than
    //one property has been updateds
    QTimer updateTimer;

    QString sha;
    QString title;
    QString url;
    QString thumbnail;
    QString type;
    int duration;
    QString genre;
    QDateTime createdAt;
    QSharedPointer<Album> album;
    QSharedPointer<Artist> artist;
};

Media::Media(const QString& url, QObject* parent)
    : QObject(parent)
    , d(new Private())
{
    d->url = url;
    d->sha = calculateSha(url);
    d->duration = 0;
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
    return d->title;
}

bool Media::setTitle(const QString& title)
{
    return updateIfChanged(d->title, title);
}

const QString& Media::url() const
{
    return d->url;
}

const QString& Media::thumbnail() const
{
    return d->thumbnail;
}

bool Media::setThumbnail(const QString& thumbnail)
{
    return updateIfChanged(d->thumbnail, thumbnail);
}

const QString& Media::sha() const
{
    return d->sha;
}

bool Media::setType(const QString& type)
{
    return updateIfChanged(d->type, type);
}

const QString& Media::type() const
{
    return d->type;
}

bool Media::setValueForRole(int role, const QVariant& value)
{
    switch (role) {
        case MediaCenter::MediaTypeRole:
            return setType(value.toString());
        case Qt::DisplayRole:
            return setTitle(value.toString());
        case MediaCenter::MediaUrlRole:
            if (d->url != value.toString()) {
                qFatal("Media URLs CANNOT be changed");
            }
            return false;
        case Qt::DecorationRole:
            return thumbnail().isEmpty() ? setThumbnail(value.toString()) : false;
        case MediaCenter::CreatedAtRole:
            return setCreatedAt(value.toDateTime());
        case MediaCenter::GenreRole:
            return setGenre(value.toString());
        case MediaCenter::DurationRole :
            return setDuration(value.toInt());
        default:
            //qWarning() << "Unknown role " << role << " for value " << value;
            return false;
    }
}

QSharedPointer<Album> Media::album() const
{
    return d->album;
}

bool Media::setAlbum(const QSharedPointer< Album >& album)
{
    return updateIfChanged(d->album, album);
}

QSharedPointer<Artist> Media::artist() const
{
    return d->artist;
}

bool Media::setArtist(const QSharedPointer< Artist >& artist)
{
    return updateIfChanged(d->artist, artist);
}

int Media::duration() const
{
    return d->duration;
}

bool Media::setDuration(int duration)
{
    return updateIfChanged(d->duration, duration);
}

QString Media::genre() const
{
    return d->genre;
}

bool Media::setGenre(const QString &genre)
{
    return updateIfChanged(d->genre, genre);
}

QDateTime Media::createdAt() const
{
    return d->createdAt;
}

bool Media::setCreatedAt(const QDateTime& createdAt)
{
    return updateIfChanged(d->createdAt, createdAt);
}

void Media::setAlbumAndUpdateRelations(const QSharedPointer<Media>& media, const QSharedPointer<Album>& album)
{
    QSharedPointer<Album> originalAlbum = media->album();
    if (setAlbum(album)) {
        if (originalAlbum) {
            originalAlbum->removeMedia(media);
        }
        album->addMedia(media);
    }
}

void Media::setArtistAndUpdateRelations(const QSharedPointer< Media >& media,
                                        const QSharedPointer< Artist >& artist)
{
    QSharedPointer<Artist> originalArtist = media->artist();
    if (setArtist(artist)) {
        if (originalArtist) {
            originalArtist->removeMedia(media);
        }
        artist->addMedia(media);
    }
}
