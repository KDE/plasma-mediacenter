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

#include <mediacenter/mediacenter.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/Vocabulary/NIE>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Vocabulary/NEXIF>
#include <Nepomuk2/Query/ResourceTypeTerm>
#include <Nepomuk2/Query/OrTerm>
#include <mediacenter/settings.h>

#include <KDebug>
#include <QImageReader>

#include <QtCore/QTimer>

MEDIACENTER_EXPORT_MEDIASOURCE(KdeMetadataMediaSource)

static const int s_queryLimit = 2000;

KdeMetadataMediaSource::KdeMetadataMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
    , m_queryServiceClient(0)
    , m_minimumImageSize(Settings().value("minImageWidth", 500).toInt())
{
    //MediaTypeRole MUST be before AlbumRole
    m_rolesRequested << Qt::DisplayRole
        << MediaCenter::ResourceIdRole
        << MediaCenter::MediaUrlRole
        << MediaCenter::MediaTypeRole
        << Qt::DecorationRole
        << MediaCenter::AlbumRole
        << MediaCenter::ArtistRole
        << MediaCenter::DurationRole
        << MediaCenter::CreatedAtRole
        << MediaCenter::GenreRole;
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
    Q_FOREACH (const Nepomuk2::Query::Result& r, results) {
        fetchValuesForResult(r);
    }
    emit newResults(results.size());
}

void KdeMetadataMediaSource::fetchValuesForResult(const Nepomuk2::Query::Result& result)
{
    const QString mimetype = mimetypeForResource(result.resource());
    QString fileType;
    if (mimetype.isEmpty() || !mimetype.contains('/')) {
        fileType = mimetype;
    } else {
        fileType = mimetype.split('/').at(0);
    }
    if (fileType == "image") {
        const QUrl imageUrl = urlForResource(result.resource());

        QImageReader image(imageUrl.toLocalFile());
        if (image.size().width() < m_minimumImageSize) {
            return;
        }
    }

    QHash<int, QVariant> values;
    Q_FOREACH(int role, m_rolesRequested) {
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
            values.insert(role, fileType);
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
        case MediaCenter::DurationRole:
            if (values.value(MediaCenter::MediaTypeRole).toString() == "audio"
                    || values.value(MediaCenter::MediaTypeRole).toString() == "video")
            {
                const int duration = result.resource().property(
                    Nepomuk2::Vocabulary::NFO::duration()).toInt();
                values.insert(role, duration);
            break;
            }
        case MediaCenter::GenreRole:
            if (values.value(MediaCenter::MediaTypeRole).toString() == "audio") {
                QString genre = result.resource().property(
                            Nepomuk2::Vocabulary::NMM::genre()).toString();
                values.insert(role, genre);
            }
            break;
        case MediaCenter::CreatedAtRole: {
            QDateTime createdDateTime;

            if (values.value(MediaCenter::MediaTypeRole).toString() == "image") {
                createdDateTime = result.resource().property(
                    Nepomuk2::Vocabulary::NEXIF::dateTimeOriginal()).toDateTime();
            }

            if (!createdDateTime.isValid()) {
                createdDateTime = result.resource().property(
                    Nepomuk2::Vocabulary::NIE::created()).toDateTime();
            }

            if (createdDateTime.isValid()) {
                values.insert(role, createdDateTime);
            }
        }
        }
    }

    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
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
