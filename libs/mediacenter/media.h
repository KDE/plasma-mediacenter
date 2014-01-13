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

#include <odb/core.hxx>
#include <odb/traits.hxx>

#include "album.h"
#include "artist.h"
#include "mediacenter.h"

#pragma db object
class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &url, QObject* parent = 0);

    const QString& sha() const;

    const QString& title () const;
    bool setTitle(const QString &title);

    const QString& url() const;

    const QString& thumbnail () const;
    bool setThumbnail(const QString &thumbnail);

    const QString& type() const;
    bool setType(const QString &type);

    bool setValueForRole(int role, const QVariant &value);
    static QString calculateSha(const QString& url);

signals:
    void updated();

private:
    explicit Media(QObject* parent = 0);
    friend class odb::access;

    #pragma db id
    QString m_sha;
    QString m_title;
    QString m_url;
    QString m_thumbnail;
    QString m_type;

    QSharedPointer<Album> m_album;
    QSharedPointer<Artist> m_artist;

    class Private;
    #pragma db transient
    Private * const d;

    inline void emitUpdate();
    void initUpdateTimer();
};

#endif // MEDIA_H
