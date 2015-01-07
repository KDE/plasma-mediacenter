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

#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QObject>
#include <QSharedPointer>
#include <QVariant>
#include <QDateTime>

#include "mediacenter_export.h"

class Album;
class Artist;

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(const QString &url, QObject* parent = 0);
    virtual ~Media();

    const QString& sha() const;

    const QString& title() const;
    bool setTitle(const QString &title);

    const QString& url() const;

    const QString& thumbnail() const;
    bool setThumbnail(const QString &thumbnail);

    const QString& type() const;
    bool setType(const QString &type);

    int duration() const;
    bool setDuration(int duration);

    QString genre() const;
    bool setGenre(const QString &genre);

    QDateTime createdAt() const;
    bool setCreatedAt(const QDateTime &createdAt);

    bool setValueForRole(int role, const QVariant &value);
    static QString calculateSha(const QString& url);

    QSharedPointer<Album> album() const;
    //FIXME: remove the first arg when this is fixed: https://bugreports.qt-project.org/browse/QTBUG-7287
    void setAlbumAndUpdateRelations(const QSharedPointer<Media> &media,
                                    const QSharedPointer<Album> &album);

    QSharedPointer<Artist> artist() const;
    //FIXME: remove the first arg when this is fixed: https://bugreports.qt-project.org/browse/QTBUG-7287
    void setArtistAndUpdateRelations(const QSharedPointer<Media> &media,
                                     const QSharedPointer<Artist> &artist);

    int rating() const;

signals:
    void updated();

private:
    class Private;
    Private * const d;

    inline void emitUpdate();
    void initUpdateTimer();

    template <class T> bool updateIfChanged(T &variable, const T &newValue);

    bool setAlbum(const QSharedPointer<Album> &album);
    bool setArtist(const QSharedPointer<Artist> &artist);
    bool setRating(const int &rating);
};

#endif // MEDIA_H
