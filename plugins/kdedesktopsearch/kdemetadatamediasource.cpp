/***********************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>          `                     *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2011 Marco Martin <notmart@gmail.com>                               *
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

#include "kdemetadatamediasource.h"

#include <libs/mediacenter/mediacenter.h>
#include <libs/mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/Vocabulary/NIE>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Query/ResourceTypeTerm>
#include <Nepomuk2/Query/OrTerm>

#include <KDebug>

#include <QtCore/QTimer>

MEDIACENTER_EXPORT_MEDIASOURCE(KdeMetadataMediaSource)

static const int s_queryLimit = 2000;

KdeMetadataMediaSource::KdeMetadataMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args), m_queryServiceClient(0)
{
    //MediaTypeRole MUST be before AlbumRole
    m_rolesRequested << Qt::DisplayRole
        << MediaCenter::ResourceIdRole
        << MediaCenter::MediaUrlRole
        << MediaCenter::MediaTypeRole
        << Qt::DecorationRole
        << MediaCenter::AlbumRole
        << MediaCenter::ArtistRole;

    moveToThread(this);
}

KdeMetadataMediaSource::~KdeMetadataMediaSource()
{
    kDebug() << "Waiting for metadata updater to quit...";
    quit();
    wait();
}

void KdeMetadataMediaSource::run()
{
    Nepomuk2::Query::ResourceTypeTerm audioTerm(
        Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Audio()));
    Nepomuk2::Query::ResourceTypeTerm videoTerm(
        Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Video()));
    Nepomuk2::Query::ResourceTypeTerm imageTerm(
        Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Image()));

    setTerm(Nepomuk2::Query::OrTerm(audioTerm, videoTerm, imageTerm));

    exec();
}

void KdeMetadataMediaSource::setTerm(const Nepomuk2::Query::Term& term)
{
    QMutexLocker locker(&m_termMutex);
    m_term = term;
    QTimer::singleShot(0, this, SLOT(runQuery()));
}

void KdeMetadataMediaSource::runQuery()
{
    Nepomuk2::Query::Query myQuery;
    if (m_queryServiceClient) {
        m_queryServiceClient->close();
        m_queryServiceClient->deleteLater();
    }
    m_queryServiceClient = new Nepomuk2::Query::QueryServiceClient(this);

    connect(m_queryServiceClient, SIGNAL(newEntries(QList<Nepomuk2::Query::Result>)),
            this, SLOT(newEntries(QList<Nepomuk2::Query::Result>)));
//     connect(queryServiceClient, SIGNAL(entriesRemoved(QList<QUrl>)),SLOT(entriesRemoved(QList<QUrl>)));
    connect(m_queryServiceClient, SIGNAL(error(QString)), SIGNAL(queryError(QString)));
    connect(m_queryServiceClient, SIGNAL(finishedListing()), SLOT(finishedListing()));

    emit reset();
    QMutexLocker locker(&m_termMutex);
    myQuery.setTerm(m_term);

    kDebug() << "SPARQL Query " << myQuery.toSparqlQuery();
    if (!m_queryServiceClient->query(myQuery)) {
        emit queryError(i18n("Could not fetch your media. Please make sure Desktop Search is enabled in System Settings."));
        return;
    }

    emit queryStarted();
}

void KdeMetadataMediaSource::newEntries(const QList< Nepomuk2::Query::Result >& results)
{
    int i = 0;
    foreach (const Nepomuk2::Query::Result& r, results) {
        fetchValuesForResult(i++, r);
    }
    emit newResults(results.size());
}

void KdeMetadataMediaSource::fetchValuesForResult(int i, const Nepomuk2::Query::Result& result)
{
    QHash<int, QVariant> values;
    foreach(int role, m_rolesRequested) {
        switch(role) {
        case Qt::DecorationRole: {
        const QString icon = result.resource().genericIcon();
        if (!icon.isEmpty()) {
            values.insert(role, icon);
            break;
        }
        values.insert(role, QString());
            break; }
        case Qt::DisplayRole:
            values.insert(role, result.resource().genericLabel());
            break;
        case MediaCenter::ResourceIdRole:
            values.insert(role, result.resource().uri());
            break;
        case MediaCenter::MediaUrlRole:
            values.insert(role, urlForResource(result.resource()));
            break;
        case MediaCenter::MediaTypeRole: {
            const QString mimetype = mimetypeForResource(result.resource());
            if (mimetype.isEmpty() || !mimetype.contains('/')) {
                values.insert(role, mimetype);
            } else {
                values.insert(role, mimetype.split('/').at(0));
            }
            break;
        }
        case MediaCenter::AlbumRole:
            if (values.value(MediaCenter::MediaTypeRole).toString() == "audio") {
                Nepomuk2::Resource album = result.resource().property(
                    Nepomuk2::Vocabulary::NMM::musicAlbum()).toResource();
                values.insert(role, album.genericLabel());
            }
            break;
        case MediaCenter::ArtistRole:
            if (values.value(MediaCenter::MediaTypeRole).toString() == "audio") {
                Nepomuk2::Resource artist = result.resource().property(
                    Nepomuk2::Vocabulary::NMM::performer()).toResource();
                values.insert(role, artist.genericLabel());
            }
            break;
        }
    }

    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
    emit gotMetadata(i, values);
}

QString KdeMetadataMediaSource::mimetypeForResource(const Nepomuk2::Resource& resource) const
{
    const QString mime = resource.property(Nepomuk2::Vocabulary::NIE::mimeType()).toString();
    if (mime.isEmpty()) {
        if (resource.type() == Nepomuk2::Vocabulary::NCO::Contact()) {
            return "artist";
        } else if (resource.type() == Nepomuk2::Vocabulary::NMM::MusicAlbum()) {
            return "album";
        }
    }
    return mime;
}

QString KdeMetadataMediaSource::urlForResource(const Nepomuk2::Resource &resource) const
{
    //Misusing URL role for returning Artist for Albums, not too bad I guess
    if (resource.type() == Nepomuk2::Vocabulary::NMM::MusicAlbum()) {
        return resource.property(Nepomuk2::Vocabulary::NMM::albumArtist()).toResource().genericLabel();
    } else {
        return resource.property(Nepomuk2::Vocabulary::NIE::url()).toUrl().toString();
    }
}

void KdeMetadataMediaSource::finishedListing()
{
    emit queryFinished();
    qobject_cast<Nepomuk2::Query::QueryServiceClient*>(sender())->close();
}

#include "kdemetadatamediasource.moc"
