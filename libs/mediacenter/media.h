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

#include "album.h"
#include "artist.h"

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject* parent = 0);
    Media(const QString &url, QObject* parent = 0);
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

    bool setValueForRole(int role, const QVariant &value);
    static QString calculateSha(const QString& url);

    const QSharedPointer<Album>& album() const;
    void setAlbum(const QSharedPointer<Album> &album);

    const QSharedPointer<Artist>& artist() const;
    void setArtist(const QSharedPointer<Artist> &artist);

    QString m_sha;
    QString m_title;
    QString m_url;
    QString m_thumbnail;
    QString m_type;
    int m_duration;
    Album::Ptr m_album;
    Artist::Ptr m_artist;

    typedef QSharedPointer<Media> Ptr;
    typedef QList<Ptr> List;

signals:
    void updated();

private:
    class Private;
    Private * const d;

    inline void emitUpdate();
    void initUpdateTimer();

    template <class T> bool updateIfChanged(T &variable, const T &newValue);
};

QX_REGISTER_PRIMARY_KEY(Media, QString)
QX_REGISTER_HPP_PMC(Media, QObject, 1)

#endif // MEDIA_H
