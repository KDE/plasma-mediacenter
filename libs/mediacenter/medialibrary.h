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
#include <odb/sqlite/query.hxx>

#include "media.h"
#include "pmcmedia.h"
#include "pmcartist.h"
#include "pmcalbum.h"

class MEDIACENTER_EXPORT MediaLibrary : public QThread
{
    Q_OBJECT
public:
    class Singleton;

    static MediaLibrary *instance();

    explicit MediaLibrary(QObject* parent = 0);
    ~MediaLibrary();

    virtual void run();

    void updateMedia(const QHash<int, QVariant> &data);
    void updateMedia(const QString &url, const QHash<int, QVariant> &data);
    QList<QSharedPointer <PmcMedia> > getMedia(const QString &type) const;
    QList< QSharedPointer< PmcAlbum > > getAlbums() const;
    QList< QSharedPointer< PmcArtist > > getArtists() const;

Q_SIGNALS:
    void newMedia(const QList< QSharedPointer<PmcMedia> > &media);
    void newAlbums(const QList< QSharedPointer<PmcAlbum> > &album);
    void newArtists(const QList< QSharedPointer<PmcArtist> > &artist);

private Q_SLOTS:
    void processRemainingRequests();
    void initDb();
    void emitNewMediaWithMediaList();

private:
    class Private;
    Private * const d;

    bool areThereUpdateRequests();
    void processNextRequest();
    QPair<QString, QHash<int, QVariant> > takeRequest();
    void updateLibrary();
    bool mediaExists(const QString &first) const;
    void addMedia(const QSharedPointer< Media > &m);
    QSharedPointer<Media> mediaForSha(const QString &sha);
    void emitNewMedia();
    bool extractAndSaveAlbumAndArtistInfo(const QPair< QString, QHash< int, QVariant > >& request, QSharedPointer< Media >& media);
    template <class X> QSharedPointer<X> loadOrCreate(const QString& name, QSharedPointer< Media >& media, odb::sqlite::query_base query_params, bool &created = bool());

    inline void addAlbum(const QSharedPointer< Album >& album);
    inline void addArtist(const QSharedPointer<Artist>& artist);
    template <class X, class Y> void addAlbumOrArtist(
        const QSharedPointer< X >& value, QMutex& mutex, QList< QSharedPointer< X > >& valueList, QList< QSharedPointer< Y > >& wrapperList, QList< QSharedPointer< Y > >& updatesList);
};

#endif // MEDIALIBRARY_H
