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

#include "medialibrary.h"
#include <KDE/KGlobal>

#include <QMutex>
#include <QVariant>
#include <QTimer>
#include <QDebug>

#include "album.h"
#include "mediacenter.h"
#include "media_odb.h"

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/query.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx>

typedef odb::query<Album> AlbumQuery;
typedef odb::result<Album> AlbumResult;

typedef odb::result<Media> MediaResult;

class MediaLibrary::Singleton
{
public:
    MediaLibrary q;
};

K_GLOBAL_STATIC( MediaLibrary::Singleton, singleton )

MediaLibrary *MediaLibrary::instance()
{
    return &( singleton->q );
}

class MediaLibrary::Private
{
public:
    Private() : session(0)
    {
    }

    ~Private()
    {
        if (session) {
            delete session;
        }
    }
    odb::core::database *db;
    odb::session *session;

    QList< QPair <QString, QHash<int, QVariant> > > updateRequests;
    QMutex updateRequestsMutex;

    QList<Media> mediaToPersist;
    QMutex mediaToPersistMutex;

    QMutex mediaMutex;
    QHash <QString, QList<QSharedPointer<PmcMedia> > > pmcMediaByType;
    QHash <QString, QSharedPointer<Media> > mediaBySha;
    QHash <QString, QSharedPointer<PmcMedia> > pmcMediaBySha;

    QList< QSharedPointer<PmcMedia> > newMediaList;
    QTimer newMediaTimer;

    QMutex albumListMutex;
    QList< QSharedPointer<Album> > albumList;
};

MediaLibrary::MediaLibrary(QObject* parent)
    : QThread(parent)
    , d(new Private())
{
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcMedia> >");

    moveToThread(this);

    d->newMediaTimer.setInterval(1000);
    d->newMediaTimer.setSingleShot(true);
    connect(&d->newMediaTimer, SIGNAL(timeout()),
            SLOT(emitNewMediaWithMediaList()));

    QTimer::singleShot(0, this , SLOT(initDb()));
    start();
}

MediaLibrary::~MediaLibrary()
{
    qDebug() << "Waiting for Media Library to finish...";
    quit();
    wait();
    delete d;
}

void MediaLibrary::run()
{
    d->session = new odb::session;
    exec();
}

void MediaLibrary::processRemainingRequests()
{
    Q_ASSERT(thread() == this);

    bool hasProcessedAnyRequest = false;
    odb::core::transaction t(d->db->begin());

    while (areThereUpdateRequests()) {
        hasProcessedAnyRequest = true;
        processNextRequest();
    }

    if (hasProcessedAnyRequest) {
        t.commit();
    }
}

void MediaLibrary::processNextRequest()
{
    QPair<QString, QHash<int, QVariant> > request = takeRequest();

//     qDebug() << "Process " << request.first;
    const QString mediaSha = Media::calculateSha(request.first);
    if (mediaExists(mediaSha)) {
        QSharedPointer<Media> media = mediaForSha(mediaSha);

        bool wasUpdated = false;
        foreach(int role, request.second.keys()) {
            if (role == MediaCenter::AlbumRole) {
                wasUpdated = wasUpdated || extractAndSaveAlbumInfo(request, media);
            } else {
                wasUpdated = wasUpdated || media->setValueForRole(role, request.second.value(role));
            }
        }

        if (wasUpdated) {
            d->db->update(media);
            qDebug() << "Updated " << media->url();
        }
    } else {
        QSharedPointer<Media> media(new Media(request.first));
        foreach(int role, request.second.keys()) {
            if (role == MediaCenter::AlbumRole) {
                extractAndSaveAlbumInfo(request, media);
            } else {
                media->setValueForRole(role, request.second.value(role));
            }
        }

        addMedia(media);
        const QString sha = d->db->persist(media);
        qDebug() << "Saved " << sha;
    }
}

bool MediaLibrary::extractAndSaveAlbumInfo(
    const QPair<QString, QHash<int, QVariant> > &request,
    const QSharedPointer<Media> &media)
{
    const QString albumName = request.second.value(MediaCenter::AlbumRole).toString();
    if (!media->album().isNull() && media->album()->name() == albumName) {
        return false;
    }

    if (!albumName.isEmpty() && albumName.length() < 250) {
        qDebug() << "Album Name " << albumName;
        AlbumResult results (d->db->query<Album>(AlbumQuery::name == albumName));
        if (results.empty()) {
            QSharedPointer<Album> album(new Album(albumName));
            media->setAlbum(album);
            d->db->persist(album);
        } else {
            QSharedPointer<Album> album(results.begin().load());
            media->setAlbum(album);
        }

        return true;
    }
    return false;
}

bool MediaLibrary::mediaExists(const QString& sha) const
{
    return d->mediaBySha.contains(sha);
}

QSharedPointer< Media > MediaLibrary::mediaForSha(const QString& sha)
{
    return d->mediaBySha.value(sha);
}

QPair< QString, QHash< int, QVariant > > MediaLibrary::takeRequest()
{
    QMutexLocker locker(&d->updateRequestsMutex);

    return d->updateRequests.takeFirst();
}

bool MediaLibrary::areThereUpdateRequests()
{
    QMutexLocker locker(&d->updateRequestsMutex);

    return !d->updateRequests.isEmpty();
}

void MediaLibrary::updateMedia(const QHash< int, QVariant >& data)
{
    updateMedia(data.value(MediaCenter::MediaUrlRole).toString(), data);
}

void MediaLibrary::updateMedia(const QString& url, const QHash< int, QVariant >& data)
{
    QMutexLocker locker(&d->updateRequestsMutex);

    d->updateRequests.append(QPair<QString, QHash< int, QVariant> >(url, data));

    QTimer::singleShot(0, this, SLOT(processRemainingRequests()));
}

void MediaLibrary::initDb()
{
    d->db = new odb::sqlite::database("plasma-mediacenter.db",
                                  SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);

    odb::connection_ptr c (d->db->connection ());
    c->execute ("PRAGMA foreign_keys=OFF");

    try {
        odb::transaction t (c->begin ());
        odb::schema_catalog::create_schema (*(d->db), "", false);
        t.commit ();
    } catch (odb::sqlite::database_exception e) {
        qDebug() << "Not recreating Schema";
    }

    c->execute ("PRAGMA foreign_keys=ON");

    updateLibrary();
    QTimer::singleShot(0, this, SLOT(processRemainingRequests()));
}

void MediaLibrary::updateLibrary()
{
    odb::connection_ptr c (d->db->connection ());
    odb::transaction t (c->begin ());

    //Media
    MediaResult mediaResults (d->db->query<Media>());

    for (MediaResult::iterator i=mediaResults.begin(); i!=mediaResults.end(); ++i) {
        QSharedPointer<Media> m = i.load();
        addMedia(m);
    }

    //Albums
    AlbumResult albums (d->db->query<Album>());

    for (AlbumResult::iterator i=albums.begin(); i!=albums.end(); ++i) {
        QSharedPointer<Album> a = i.load();
        addAlbum(a);
    }
}

void MediaLibrary::addMedia(const QSharedPointer< Media >& m)
{
    QMutexLocker l(&d->mediaMutex);

    d->mediaBySha.insert(m->sha(), m);
    QSharedPointer<PmcMedia> pmcMedia(new PmcMedia(m));

    d->pmcMediaBySha.insert(m->sha(), pmcMedia);
    d->pmcMediaByType[m->type()].append(pmcMedia);

    d->newMediaList.append(pmcMedia);
    emitNewMedia();
}

void MediaLibrary::addAlbum(const QSharedPointer< Album >& a)
{
    QMutexLocker l(&d->albumListMutex);

    qDebug() << a->name();
    d->albumList.append(a);
}

QList< QSharedPointer<PmcMedia> > MediaLibrary::getMedia(const QString& type)
{
    QMutexLocker l(&d->mediaMutex);

    return d->pmcMediaByType.value(type);
}

void MediaLibrary::emitNewMedia()
{
    if (!d->newMediaTimer.isActive()) {
        d->newMediaTimer.start();
    }
}

void MediaLibrary::emitNewMediaWithMediaList()
{
    qDebug() << "Emitting new media";
    emit newMedia(d->newMediaList);
    d->newMediaList.clear();
}

QList< QSharedPointer< Album > > MediaLibrary::getAlbums()
{
    QMutexLocker l(&d->albumListMutex);

    return d->albumList;
}
