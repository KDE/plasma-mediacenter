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

#include <KDebug>

MetadataUpdater::MetadataUpdater(const QList< int >& rolesRequested, QObject* parent)
    : QThread(parent), m_rolesRequested(rolesRequested), m_shouldQuit(false)
{
}

MetadataUpdater::~MetadataUpdater()
{
}

void MetadataUpdater::run()
{
    while (!shouldQuit()) {
        while (areThereResultsToProcess()) {
            const QPersistentModelIndex &index = nextIndexToProcess();
            fetchValuesForResult(index, resultForIndex(index));
        }
        msleep(100);
    }
}

void MetadataUpdater::fetchValuesForResult(const QPersistentModelIndex &index, const Nepomuk::Query::Result& result)
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

    emit gotMetadata(index, values);
}

void MetadataUpdater::addResultToQueue(const QPersistentModelIndex& index, const Nepomuk::Query::Result& result)
{
    QMutexLocker lock(&m_resultsMutex);
    m_indices.append(index);
    m_results.insert(index, result);
}

QPersistentModelIndex MetadataUpdater::nextIndexToProcess()
{
    QMutexLocker lock(&m_resultsMutex);
    return m_indices.takeFirst();
}

Nepomuk::Query::Result MetadataUpdater::resultForIndex(const QPersistentModelIndex& index)
{
    QMutexLocker lock(&m_resultsMutex);
    return m_results.take(index);
}

bool MetadataUpdater::areThereResultsToProcess()
{
    QMutexLocker lock(&m_resultsMutex);
    return !m_indices.empty();
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
