/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef ALBUM_H
#define ALBUM_H

#include <QString>
#include <QList>
#include <QWeakPointer>

class Artist;
class Media;

class Album : public QObject
{
    Q_OBJECT
public:
    explicit Album(const QString &name, const QSharedPointer<Artist> &artist);

    const QString& name() const;

    QString albumArtist() const;

    void addMedia(const QSharedPointer<Media> &media);
    void removeMedia(const QSharedPointer<Media> &media);
    QList< QSharedPointer<Media> > media() const;

signals:
    void updated();

private:
    QSharedPointer<Artist> m_albumArtist;
    QString m_name;
    QList< QSharedPointer<Media> > m_media;
};

#endif // ALBUM_H
