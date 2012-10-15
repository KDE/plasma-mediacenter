/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
 *   Copyright 2011 Marco Martin <notmart@gmail.com>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "metadataupdater.h"

#include "mediacenter.h"

#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <nepomuk/nie.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/nfo.h>

#include <KDebug>

#include <QtCore/QTimer>

MetadataUpdater::MetadataUpdater(const QList< int >& rolesRequested, QObject* parent)
    : QThread(parent), m_rolesRequested(rolesRequested), m_shouldQuit(false), m_termChanged(false)
{
}

MetadataUpdater::~MetadataUpdater()
{
}

void MetadataUpdater::run()
{
    QTimer::singleShot(0, this, SLOT(processPendingIndices()));
    exec();
}

void MetadataUpdater::setTerm(const Nepomuk::Query::Term& term)
{
    QMutexLocker locker(&m_termMutex);
    m_term = term;
    m_termChanged = true;
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
    m_resultList.clear();

    Nepomuk::Query::Query myQuery;
    myQuery.setLimit(2000);
    Nepomuk::Query::QueryServiceClient *queryServiceClient = new Nepomuk::Query::QueryServiceClient(this);

    connect(queryServiceClient, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
            this, SLOT(newEntries(QList<Nepomuk::Query::Result>)));
//     connect(queryServiceClient, SIGNAL(entriesRemoved(QList<QUrl>)),SLOT(entriesRemoved(QList<QUrl>)));
//     connect(queryServiceClient, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(queryServiceClient, SIGNAL(finishedListing()), SLOT(finishedListing()));

    QMutexLocker locker(&m_termMutex);
    myQuery.setTerm(m_term);
    kDebug()<< "SSparql query: " << myQuery.toSparqlQuery();

    queryServiceClient->query(myQuery);
}

void MetadataUpdater::newEntries(const QList< Nepomuk::Query::Result >& results)
{
    m_resultList.append(results);
    emit newResults(results.size());
}

void MetadataUpdater::processPendingIndices()
{
    if (shouldQuit())
        exit();

    if (hasTermChanged())
        runQuery();

    if (areThereResultsToProcess()) {
        if (shouldQuit())
            exit();

        const int i = nextIndexToProcess();
        fetchValuesForResult(i, resultForRow(i));
    }
    QTimer::singleShot(100, this, SLOT(processPendingIndices()));
}

void MetadataUpdater::fetchValuesForResult(int i, const Nepomuk::Query::Result& result)
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
        values.insert(role, "kde");
            break; }
        case Qt::DisplayRole:
            values.insert(role, result.resource().genericLabel());
            break;
        case MediaCenter::ResourceIdRole:
            values.insert(role, result.resource().resourceUri());
            break;
        case MediaCenter::MediaUrlRole:
            values.insert(role, urlForResource(result.resource()));
            break;
        case MediaCenter::MediaTypeRole:
            //TODO: Put null checks here
            values.insert(role, mimetypeForResource(result.resource()).split("/").at(0));
            break;
        }
    }

    emit gotMetadata(i, values);
}

void MetadataUpdater::fetchMetadata(int row)
{
    QMutexLocker lock(&m_indicesMutex);
    m_indices.append(row);
}

void MetadataUpdater::fetchMetadata(const QList< int >& rows)
{
    QMutexLocker lock(&m_indicesMutex);
    m_indices.append(rows);
}

int MetadataUpdater::nextIndexToProcess()
{
    QMutexLocker lock(&m_indicesMutex);
    return m_indices.takeLast();
}

Nepomuk::Query::Result MetadataUpdater::resultForRow(int row)
{
    QMutexLocker lock(&m_resultsMutex);
    return m_resultList.at(row);
}

bool MetadataUpdater::areThereResultsToProcess()
{
    QMutexLocker lock(&m_indicesMutex);
    return !m_indices.isEmpty();
}

QString MetadataUpdater::mimetypeForResource(const Nepomuk::Resource& resource) const
{
    const QString mime = resource.property(Nepomuk::Vocabulary::NIE::mimeType()).toString();
    return mime;
}

QString MetadataUpdater::urlForResource(const Nepomuk::Resource &resource) const
{
    return resource.property(Nepomuk::Vocabulary::NIE::url()).toUrl().toString();
}

void MetadataUpdater::quit()
{
    QMutexLocker locker(&m_quitMutex);
    m_shouldQuit = true;
}

bool MetadataUpdater::shouldQuit()
{
    QMutexLocker locker(&m_quitMutex);
    return m_shouldQuit;
}

void MetadataUpdater::finishedListing()
{
    qobject_cast<Nepomuk::Query::QueryServiceClient*>(sender())->close();
}

#include "metadataupdater.moc"
