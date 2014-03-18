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

#include <QVariant>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QPointer>

#include "media.h"

#include "mediacenter.h"
#include "medialibrarywrappercache.h"
#include "mediavalidator.h"

#include "pmcmedia.h"
#include "pmcalbum.h"
#include "pmcartist.h"

static const QString DB_NAME = "plasma-mediacenter.sqlite";

class MediaLibrary::Private
{
public:
    Private() : db(0)/*, session(0)*/ { }

    ~Private()
    {
//         if (session) {
//             delete session;
//         }
//
        if (db) {
            db->deleteSingleton();
        }
    }
    qx::QxSqlDatabase *db;

    QList< QPair <QString, QHash<int, QVariant> > > updateRequests;
    QMutex updateRequestsMutex;

    QList<Media> mediaToPersist;
    QMutex mediaToPersistMutex;

    QMutex mediaMutex;
    QHash <QString, QSharedPointer<Media> > mediaBySha;

    QList< QSharedPointer<PmcMedia> > newMediaList;
    QList< QSharedPointer<PmcAlbum> > newAlbumList;
    QList< QSharedPointer<PmcArtist> > newArtistList;
    QPointer<QTimer> newMediaTimer;

    QMutex albumListMutex;
    QList< QSharedPointer<Album> > albumList;

    QMutex artistListMutex;
    QList<QSharedPointer<Artist> > artistList;

    MediaLibraryWrapperCache *wrapperCache;
    MediaValidator *mediaValidator;
};

MediaLibrary::MediaLibrary(MediaValidator* mediaValidator, QObject* parent)
    : QThread(parent)
    , d(new Private())
{
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcMedia> >");
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcAlbum> >");
    qRegisterMetaType< QList< QSharedPointer<PmcMedia> > >("QList< QSharedPointer<PmcArtist> >");

    moveToThread(this);

    d->wrapperCache = new MediaLibraryWrapperCache(this);
    d->mediaValidator = mediaValidator ? mediaValidator : new MediaValidator(this);
}

void MediaLibrary::init()
{
    d->newMediaTimer = new QTimer(this);
    d->newMediaTimer->setInterval(1000);
    d->newMediaTimer->setSingleShot(true);
    connect(d->newMediaTimer.data(), SIGNAL(timeout()), SLOT(emitNewMediaWithMediaList()));
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
    init();
    initDb();
    exec();
}

void MediaLibrary::processRemainingRequests()
{
    Q_ASSERT(thread() == this);

    bool hasProcessedAnyRequest = false;

    if (!areThereUpdateRequests()) {
        return;
    }

    d->db->getDatabase().transaction();

    while (areThereUpdateRequests()) {
        processNextRequest();
    }

    d->db->getDatabase().commit();
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
            } else if (role == MediaCenter::ArtistRole) {
                wasUpdated = wasUpdated || extractAndSaveArtistInfo(request, media);
            } else if (role == MediaCenter::DurationRole){
                wasUpdated = wasUpdated || extractAndSaveDurationInfo(request, media);
            } else {
                wasUpdated = wasUpdated || media->setValueForRole(role, request.second.value(role));
            }
        }

        if (wasUpdated) {
            qx::dao::update(media);
            qDebug() << "Updated " << media->url();
        }
    } else {
        if (d->mediaValidator->fileWithUrlExists(request.first)) {
            QSharedPointer<Media> media(new Media(request.first));
            foreach(int role, request.second.keys()) {
                if (role == MediaCenter::AlbumRole) {
                    extractAndSaveAlbumInfo(request, media);
                } else if(role == MediaCenter::ArtistRole) {
                    extractAndSaveArtistInfo(request, media);
                } else if(role == MediaCenter::DurationRole) {
                    extractAndSaveDurationInfo(request, media);
                } else {
                    media->setValueForRole(role, request.second.value(role));
                }
            }

            const QString sha = persistMedia(media);
            addMedia(media);
            qDebug() << "Saved " << sha;
        }
    }
}

QString MediaLibrary::persistMedia(const QSharedPointer< Media > &media)
{
    QSqlError daoError = qx::dao::insert(*media);
    const QString sha = media->sha();

    if (hasError(daoError)) {
        qWarning() << "Could not insert " << sha;
        return QString();
    }

    if (media->album()) {
        QSharedPointer<Album> album(new Album(media->album()->name()));
        qx::dao::fetch_by_id_with_relation("*->*", album);
        media->setAlbum(album);
    }
    if (media->artist()) {
        QSharedPointer<Artist> artist(new Artist(media->artist()->name()));
        qx::dao::fetch_by_id_with_relation("*->*", artist);
        media->setArtist(artist);
    }

    return sha;
}

bool MediaLibrary::extractAndSaveArtistInfo(
    const QPair< QString, QHash< int, QVariant > >& request,
    QSharedPointer< Media >& media)
{
    QString artistName = request.second.value(MediaCenter::ArtistRole).toString();

    if (artistName.isEmpty()) {
        artistName = "Unknown Artist";
    }
    if (!media->artist().isNull() && media->artist()->name() == artistName) {
        return false;
    }

    QSharedPointer<Artist> artist = loadOrCreate<Artist>(artistName);

    media->setArtist(artist);
    return true;
}

bool MediaLibrary::extractAndSaveAlbumInfo(
    const QPair<QString, QHash<int, QVariant> > &request,
    QSharedPointer<Media> &media)
{
    QString albumName = request.second.value(MediaCenter::AlbumRole).toString();
    QString artistName = request.second.value(MediaCenter::ArtistRole).toString();

    if (albumName.isEmpty()){
        albumName = "Unknown Album";
    }
    if (artistName.isEmpty()) {
        artistName = media->artist()->name();
    }
    if (!media->album().isNull() && media->album()->name() == albumName
        && media->album()->albumArtist() == artistName) {
        return false;
    }

    QSharedPointer<Album> album = loadOrCreate<Album>(albumName);

    media->setAlbum(album);
    return true;
}

bool MediaLibrary::extractAndSaveDurationInfo(
    const QPair< QString, QHash< int, QVariant > > request,
    QSharedPointer< Media >& media)
{
    const int duration = request.second.value(MediaCenter::DurationRole).toInt();

    return duration < 0 ? false : media->setDuration(duration);
}

template <class T>
QSharedPointer< T > MediaLibrary::loadOrCreate(const QString& id)
{
    QSharedPointer<T> object(new T(id));
    QSqlError daoError = qx::dao::fetch_by_id(object);

    if (hasError(daoError)) {
        daoError = qx::dao::insert(object);

        if (hasError(daoError)) {
            qWarning() << "Error inserting " << id << ": " << daoError.text();
            qFatal("Failed while inserting into DB, see above for error");
        }
    }

    return object;
}

bool MediaLibrary::mediaExists(const QString& sha) const
{
    return d->mediaBySha.contains(sha);
}

QSharedPointer< Media > MediaLibrary::mediaForSha(const QString& sha) const
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
    bool dbExists = QFile::exists(DB_NAME);

    d->db = qx::QxSqlDatabase::getSingleton();
    d->db->setDriverName("QSQLITE");
    d->db->setDatabaseName("./plasma-mediacenter.sqlite");

    //TODO: Just the existence of the db file might not be enough proof that the
    // database in the file is in a usable state
    if (!dbExists) {
        qDebug() << "Creating database tables";
        bool error = false;
        error = error || hasError(qx::dao::create_table<Artist>());
        error = error || hasError(qx::dao::create_table<Album>());
        error = error || hasError(qx::dao::create_table<Media>());

        if (error) {
            qFatal("Failed to create necessary tables");
        }
    }

    updateLibrary();
//     QTimer::singleShot(0, this, SLOT(processRemainingRequests()));
}

void MediaLibrary::updateLibrary()
{
    if (d->db->getDatabase().transaction()) {
        Media::List mediaList;
        qx::dao::fetch_all_with_all_relation(mediaList);

        for (Media::List::iterator i=mediaList.begin(); i!=mediaList.end(); ++i) {
            Media::Ptr m = *i;
            if (d->mediaValidator->fileWithUrlExists(m->url())) {
                addMedia(m);
            } else {
                qx::dao::delete_by_id(m);
            }
        }

        d->db->getDatabase().commit();
        emit initialized();
    } else {
        qFatal("Failed to create a transaction");
    }
}

void MediaLibrary::addMedia(const QSharedPointer< Media >& m)
{
    QMutexLocker l(&d->mediaMutex);

    d->mediaBySha.insert(m->sha(), m);
    QSharedPointer<PmcMedia> pmcMedia(new PmcMedia(m));

    d->newMediaList.append(pmcMedia);

    addAlbum(m->album());
    addArtist(m->artist());
    emitNewMedia();
}

void MediaLibrary::addAlbum(const QSharedPointer< Album >& album)
{
    addAlbumOrArtist<Album, PmcAlbum>(album, d->albumListMutex, d->albumList,
                                      d->newAlbumList);
}

void MediaLibrary::addArtist(const QSharedPointer< Artist >& artist)
{
    addAlbumOrArtist<Artist, PmcArtist>(artist, d->artistListMutex,
                                        d->artistList, d->newArtistList);
}

template <class X, class Y>
void MediaLibrary::addAlbumOrArtist(const QSharedPointer< X >& value,
                                    QMutex &mutex,
                                    QList< QSharedPointer< X > >& valueList,
                                    QList< QSharedPointer< Y > >& updatesList)
{
    if (value.isNull()) return;
    QMutexLocker l(&mutex);

    if (!valueList.contains(value)) {
        QSharedPointer<Y> pa(new Y(value));
        valueList.append(value);

        updatesList.append(pa);
        emitNewMedia();
    }
}

QList< QSharedPointer<PmcMedia> > MediaLibrary::getMedia(const QString& type) const
{
    QMutexLocker l(&d->mediaMutex);
    return d->wrapperCache->getMedia(type);
}

void MediaLibrary::emitNewMedia()
{
    if (!d->newMediaTimer->isActive()) {
        d->newMediaTimer->start();
    }
}

#define PMC_ML_EMIT_IF_NEEDED(mutex, list, signal) \
    { \
        QMutexLocker l(&mutex); \
        if (!list.isEmpty()) { \
            qDebug() << "Emitting " << SIGNAL(signal); \
            emit signal(list); \
            list.clear(); \
        } \
    } \

void MediaLibrary::emitNewMediaWithMediaList()
{
    PMC_ML_EMIT_IF_NEEDED(d->mediaMutex, d->newMediaList, newMedia)
    PMC_ML_EMIT_IF_NEEDED(d->albumListMutex, d->newAlbumList, newAlbums)
    PMC_ML_EMIT_IF_NEEDED(d->artistListMutex, d->newArtistList, newArtists)
}

QList< QSharedPointer< PmcAlbum > > MediaLibrary::getAlbums() const
{
    QMutexLocker l(&d->albumListMutex);
    return d->wrapperCache->getAlbums();
}

QList< QSharedPointer< PmcArtist > > MediaLibrary::getArtists() const
{
    QMutexLocker l(&d->artistListMutex);
    return d->wrapperCache->getArtists();
}

bool MediaLibrary::hasError(const QSqlError& sqlError)
{
    return sqlError.isValid();
}

QSharedPointer< PmcMedia > MediaLibrary::mediaForUrl(const QString& url) const
{
    QSharedPointer<Media> media = mediaForSha(Media::calculateSha(url));
    return media.isNull() ? QSharedPointer<PmcMedia>()
                            : QSharedPointer<PmcMedia>(new PmcMedia(media));
}
