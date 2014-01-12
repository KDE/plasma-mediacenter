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

#include "mediacenter.h"
#include "media_odb.h"

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/query.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>
#include <odb/sqlite/exceptions.hxx>

typedef odb::query<Media> MediaQuery;
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
    odb::core::database *db;

    QList< QPair <QString, QHash<int, QVariant> > > updateRequests;
    QMutex updateRequestsMutex;

    QList<Media> mediaToPersist;
    QMutex mediaToPersistMutex;

    QHash <QString, QList<QSharedPointer<Media> > > mediaByType;
    QHash <QString, QSharedPointer<Media> > mediaBySha;
};

MediaLibrary::MediaLibrary(QObject* parent)
    : QThread(parent)
    , d(new Private())
{
    moveToThread(this);
    QTimer::singleShot(0, this , SLOT(initDb()));
    start();
}

MediaLibrary::~MediaLibrary()
{
    quit();
    wait(5000);
    delete d;
}

void MediaLibrary::run()
{
    QTimer::singleShot(0, this, SLOT(processRemainingRequests()));
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

    qDebug() << "Processing " << request.first;

    const QString mediaSha = Media::calculateSha(request.first);
    if (mediaExists(mediaSha)) {
        QSharedPointer<Media> media = mediaForSha(mediaSha);
        media->setTitle(request.second.value(Qt::DisplayRole).toString());
        media->setThumbnail(request.second.value(Qt::DecorationRole).toString());
        media->setType(request.second.value(MediaCenter::MediaTypeRole).toString());

        d->db->update(media);
        qDebug() << "Updated " << media->url();
    } else {
        QSharedPointer<Media> media(
            new Media(request.second.value(MediaCenter::MediaTypeRole).toString(),
                      request.second.value(Qt::DisplayRole).toString(),
                      request.first,
                      request.second.value(Qt::DecorationRole).toString()));
        addMedia(media);
        const QString sha = d->db->persist(media);
        qDebug() << "Saved " << sha;
    }
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
}

void MediaLibrary::updateLibrary()
{
    odb::connection_ptr c (d->db->connection ());
    odb::transaction t (c->begin ());

    MediaResult results (d->db->query<Media>());

    for (MediaResult::iterator i=results.begin(); i!=results.end(); ++i) {
        QSharedPointer<Media> m = i.load();
        addMedia(m);
    }
}

void MediaLibrary::addMedia(const QSharedPointer< Media >& m)
{
    d->mediaBySha.insert(m->sha(), m);
    d->mediaByType[m->type()].append(m);
}

QList< QSharedPointer<PmcMedia> > MediaLibrary::getMedia(const QString& type)
{
    QList<QSharedPointer<PmcMedia> > pmcMedia;
    foreach (QSharedPointer<Media> m, d->mediaByType.value(type)) {
        QSharedPointer<PmcMedia> pm = QSharedPointer<PmcMedia> (new PmcMedia(m));
        pmcMedia.append(pm);
    }
    return pmcMedia;
}
