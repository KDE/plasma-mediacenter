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

#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include "mediacenter_export.h"

#include <QHash>
#include <QThread>
#include <QSharedPointer>
#include <QMutex>
#include <QVariant>

class Artist;
class Album;
class Media;
class PmcArtist;
class PmcAlbum;
class PmcMedia;
class MediaValidator;

class MEDIACENTER_EXPORT MediaLibrary : public QThread
{
    Q_OBJECT
public:
    explicit MediaLibrary(MediaValidator *mediaValidator = 0, QObject* parent = 0);
    virtual ~MediaLibrary();

    void run() Q_DECL_OVERRIDE;

    void updateMedia(const QHash<int, QVariant> &data);
    void updateMedia(const QString &url, const QHash<int, QVariant> &data);
    void removeMedia(const QString &url);
    QList<QSharedPointer <PmcMedia> > getMedia(const QString &type) const;
    QList< QSharedPointer< PmcAlbum > > getAlbums() const;
    QList< QSharedPointer< PmcArtist > > getArtists() const;

    QSharedPointer<PmcMedia> mediaForUrl(const QString &url) const;
    void setTestMode(bool mode);

Q_SIGNALS:
    void initialized();
    void imageFetched(const QVariant &identifier, const QString &artistName);
    void newMedia(const QList< QSharedPointer<PmcMedia> > &media);
    void newAlbums(const QList< QSharedPointer<PmcAlbum> > &album);
    void newArtists(const QList< QSharedPointer<PmcArtist> > &artist);
    void mediaRemoved(QSharedPointer<PmcMedia> media);

private Q_SLOTS:
    void init();
    void processRemainingRequests();
    void emitNewMediaWithMediaList();

private:
    class Private;
    Private * const d;

    bool areThereUpdateRequests();
    void processNextRequest();
    QPair<QString, QHash<int, QVariant> > takeRequest();
    bool mediaExists(const QString &first) const;
    void addMedia(const QSharedPointer< Media > &m);
    QSharedPointer<Media> mediaForSha(const QString &sha) const;
    void emitNewMedia();
    bool extractAndSaveAlbumInfo(const QPair< QString, QHash< int, QVariant > >& request, QSharedPointer< Media >& media);
    bool extractAndSaveArtistInfo(const QPair< QString, QHash< int, QVariant > >& request, QSharedPointer< Media >& media);

    inline void addAlbum(const QSharedPointer< Album >& album);
    inline void addArtist(const QSharedPointer<Artist>& artist);
    template <class X, class Y> void addAlbumOrArtist(
        const QSharedPointer< X >& value, QMutex& mutex, QList< QSharedPointer< X > >& valueList, QList< QSharedPointer< Y > >& updatesList);
    void emitNewAlbumOrArtistIfNeeded(QSharedPointer< Media > media);
};

#endif // MEDIALIBRARY_H
