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

#include "metadataupdater.h"

#include "mediacenter.h"
#include "medialibrary.h"

#include <Nepomuk2/Resource>
#include <Nepomuk2/Variant>
#include <Nepomuk2/Vocabulary/NIE>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Vocabulary/NCO>
#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Query/ResourceTypeTerm>

#include <KDebug>

#include <QtCore/QTimer>

static const int s_queryLimit = 2000;

MetadataUpdater::MetadataUpdater(const QList< int >& rolesRequested, QObject* parent)
    : QThread(parent), m_rolesRequested(rolesRequested), m_termChanged(false), m_queryServiceClient(0)
{
    moveToThread(this);
}

MetadataUpdater::~MetadataUpdater()
{
    deleteLater();
}

void MetadataUpdater::run()
{
    QTimer::singleShot(0, this, SLOT(processPendingIndices()));
    exec();
}

void MetadataUpdater::setTerm(const Nepomuk2::Query::Term& term)
{
    QMutexLocker locker(&m_termMutex);
    m_term = term;
    m_termChanged = true;
    QTimer::singleShot(0, this, SLOT(processPendingIndices()));
}

bool MetadataUpdater::hasTermChanged()
{
    QMutexLocker locker(&m_termMutex);
    bool isChanged = m_termChanged;
    if (isChanged) {
        m_termChanged = false;
    }
    return isChanged;
}

void MetadataUpdater::runQuery()
{
    m_indices.clear();
    m_resultList.clear();

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

void MetadataUpdater::newEntries(const QList< Nepomuk2::Query::Result >& results)
{
    m_resultList.append(results);
    emit newResults(results.size());
}

void MetadataUpdater::processPendingIndices()
{
    Q_ASSERT(thread() == this);

    if (hasTermChanged())
        runQuery();

    if (areThereIndicesToProcess()) {
        const int i = nextIndexToProcess();
        fetchValuesForResult(i, resultForRow(i));
        QTimer::singleShot(10, this, SLOT(processPendingIndices()));
    }
}

void MetadataUpdater::fetchValuesForResult(int i, const Nepomuk2::Query::Result& result)
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
        case MediaCenter::MediaTypeRole:
            const QString mimetype = mimetypeForResource(result.resource());
            if (mimetype.isEmpty() || !mimetype.contains('/')) {
                values.insert(role, mimetype);
            } else {
                values.insert(role, mimetype.split('/').at(0));
            }
            break;
        }
    }

    MediaLibrary::instance()->updateMedia(values);
    emit gotMetadata(i, values);
}

void MetadataUpdater::fetchMetadata(int row)
{
    QMutexLocker lock(&m_indicesMutex);
    m_indices.append(row);
    QTimer::singleShot(0, this, SLOT(processPendingIndices()));
}

void MetadataUpdater::fetchMetadata(const QList< int >& rows)
{
    QMutexLocker lock(&m_indicesMutex);
    m_indices.append(rows);
    QTimer::singleShot(0, this, SLOT(processPendingIndices()));
}

int MetadataUpdater::nextIndexToProcess()
{
    QMutexLocker lock(&m_indicesMutex);
    return m_indices.takeLast();
}

Nepomuk2::Query::Result MetadataUpdater::resultForRow(int row)
{
    QMutexLocker lock(&m_resultsMutex);
    if (row >= m_resultList.size())
        return Nepomuk2::Query::Result();
    return m_resultList.at(row);
}

bool MetadataUpdater::areThereIndicesToProcess()
{
    QMutexLocker lock(&m_indicesMutex);
    return !m_indices.isEmpty();
}

QString MetadataUpdater::mimetypeForResource(const Nepomuk2::Resource& resource) const
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

QString MetadataUpdater::urlForResource(const Nepomuk2::Resource &resource) const
{
    //Misusing URL role for returning Artist for Albums, not too bad I guess
    if (resource.type() == Nepomuk2::Vocabulary::NMM::MusicAlbum()) {
        return resource.property(Nepomuk2::Vocabulary::NMM::albumArtist()).toResource().genericLabel();
    } else {
        return resource.property(Nepomuk2::Vocabulary::NIE::url()).toUrl().toString();
    }
}

void MetadataUpdater::finishedListing()
{
    emit queryFinished();
    qobject_cast<Nepomuk2::Query::QueryServiceClient*>(sender())->close();
}

#include "metadataupdater.moc"
