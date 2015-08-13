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

#ifndef MEDIALIBRARYWRAPPERCACHE_H
#define MEDIALIBRARYWRAPPERCACHE_H

#include <QObject>
#include <QSharedPointer>

class PmcMedia;
class PmcAlbum;
class PmcArtist;
class MediaLibrary;

class MediaLibraryWrapperCache : public QObject
{
    Q_OBJECT
public:
    explicit MediaLibraryWrapperCache(MediaLibrary *parent);
    virtual ~MediaLibraryWrapperCache();

    QList< QSharedPointer< PmcAlbum > > getAlbums() const;
    QList< QSharedPointer< PmcArtist > > getArtists() const;
    QList< QSharedPointer< PmcMedia > > getMedia(const QString &type);

private slots:
    void saveNewAlbums(const QList< QSharedPointer< PmcAlbum > >& albums);
    void saveNewArtists(const QList< QSharedPointer< PmcArtist > >& artists);
    void removeMediaRef(QSharedPointer<PmcMedia> media);
    void removeAlbumRef();
    void removeArtistRef();

private:
    class Private;
    Private * const d;
public slots:
    void saveNewMedia(const QList< QSharedPointer< PmcMedia > >& media);
};

#endif // MEDIALIBRARYWRAPPERCACHE_H
