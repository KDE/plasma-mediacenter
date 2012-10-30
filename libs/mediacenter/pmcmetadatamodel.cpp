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

#include "pmcmetadatamodel.h"
#include "pmcimagecache.h"
#include "pmcimageprovider.h"
#include "metadataupdater.h"

#include <nepomuk/comparisonterm.h>
#include <nepomuk/query.h>
#include <nepomuk/nie.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/nfo.h>
#include <nepomuk/resource.h>
#include <nepomuk/andterm.h>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Variant>
#include <nepomuk/queryparser.h>

#include <KIO/PreviewJob>
#include <KDebug>

#include <QtCore/QTimer>

class PmcMetadataModel::Private
{
public:
    Private()
        : thumbnailerPlugins(new QStringList(KIO::PreviewJob::availablePlugins()))
        , numberOfEntries(0)
        , isSearchTermValid(false)
    {
    }
    Nepomuk::Query::Term term;
    Nepomuk::Query::Term searchTerm;
    bool isSearchTermValid;
    QList< Nepomuk::Query::Result > queryResults;

    //Thumbnail stuff
    const QStringList *thumbnailerPlugins;
    QHash<KUrl, QPersistentModelIndex> filesToPreview;
    QSize thumbnailSize;
    QHash<KUrl, QPersistentModelIndex> previewJobs;
    QTimer previewTimer;
    QTimer updateTimer;
    QTimer metadataFetchTimer;
    Nepomuk::Query::ResourceTypeTerm resourceTypeTerm;
    QHash<int, QHash<int, QVariant> > metadataValues;
    MetadataUpdater *metadataUpdater;
    int numberOfEntries;
    QList<int> rowsToFetchMetadataFor;
    QStringList mediaUrlWhichFailedThumbnailGeneration;
};

PmcMetadataModel::PmcMetadataModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private())
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    d->previewTimer.setSingleShot(true);
    connect(&d->previewTimer, SIGNAL(timeout()), SLOT(delayedPreview()));
    d->updateTimer.setSingleShot(true);
    connect(&d->updateTimer, SIGNAL(timeout()), SLOT(updateModel()));
    d->metadataFetchTimer.setSingleShot(true);
    connect(&d->metadataFetchTimer, SIGNAL(timeout()), SLOT(fetchMetadata()));

    d->thumbnailSize = QSize(512, 512);

    qRegisterMetaType< QHash<int,QVariant> >("QHash<int,QVariant>");
    d->metadataUpdater = new MetadataUpdater(rolesNeeded());
    connect(d->metadataUpdater, SIGNAL(gotMetadata(int, QHash<int,QVariant>)),
            SLOT(saveMetadata(int,QHash<int,QVariant>)));
    connect(d->metadataUpdater, SIGNAL(newResults(int)), SLOT(newEntries(int)));
    connect(d->metadataUpdater, SIGNAL(reset()), SLOT(handleUpdaterReset()));
    d->metadataUpdater->start(QThread::IdlePriority);
}

PmcMetadataModel::~PmcMetadataModel()
{
    d->metadataUpdater->quit();
    kDebug() << "Waiting for metadata thread to quit...";
    d->metadataUpdater->wait();
    delete d;
}

void PmcMetadataModel::updateModel()
{
    QList<Nepomuk::Query::Term> listOfTerms;
    listOfTerms.append(d->term);

    if (d->resourceTypeTerm.isValid()) {
        Nepomuk::Query::ComparisonTerm sortTerm(Nepomuk::Vocabulary::NIE::lastModified(), Nepomuk::Query::Term());
        sortTerm.setSortWeight(1, Qt::DescendingOrder);
        listOfTerms.append(sortTerm);
    }

    if (d->isSearchTermValid) {
        listOfTerms.append(d->searchTerm);
    }

    d->metadataUpdater->setTerm(Nepomuk::Query::AndTerm(listOfTerms));
}

void PmcMetadataModel::showMediaType(MediaCenter::MediaType mediaType)
{
    switch (mediaType) {
        case MediaCenter::Music:
            d->resourceTypeTerm = Nepomuk::Query::ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Audio());
	    break;
        case MediaCenter::Picture:
            d->resourceTypeTerm = Nepomuk::Query::ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Image());
	    break;
        case MediaCenter::Video:
            d->resourceTypeTerm = Nepomuk::Query::ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Video());
    }
    clearAllFilters();
    d->updateTimer.start(100);
}

void PmcMetadataModel::showMediaForProperty(Nepomuk::Types::Property property)
{
    Nepomuk::Query::ComparisonTerm ct(property, Nepomuk::Query::Term());
    ct.setInverted(true);
    setTerm(ct);
}

void PmcMetadataModel::setTerm(const Nepomuk::Query::Term& term)
{
     d->term = term;
     d->updateTimer.start(100);
}

void PmcMetadataModel::addTerm(const Nepomuk::Query::Term& term)
{
    d->term = Nepomuk::Query::AndTerm(d->term, term);
    d->updateTimer.start(100);
}

QVariant PmcMetadataModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= rowCount())
        return QVariant();
    if (!d->metadataValues.keys().contains(index.row())) {
        d->rowsToFetchMetadataFor.append(index.row());
        d->metadataFetchTimer.start(100);
        return QVariant();
    }

    QVariant metadataValue = d->metadataValues.value(index.row()).value(role);
    switch(role) {
    case MediaCenter::ResourceIdRole:
    case MediaCenter::MediaUrlRole:
    case MediaCenter::MediaTypeRole:
    case Qt::DecorationRole:
    case Qt::DisplayRole:
        return metadataValue;
    case MediaCenter::MediaThumbnailRole:
        if (d->metadataValues.value(index.row()).value(MediaCenter::MediaTypeRole) == "video") {
            KUrl url = d->metadataValues.value(index.row()).value(MediaCenter::MediaUrlRole).toUrl();
            if (d->mediaUrlWhichFailedThumbnailGeneration.contains(url.prettyUrl()))
                return "image-missing";
            return const_cast<PmcMetadataModel*>(this)->fetchPreview(url, index);
        }
    }

    return QVariant();
}

void PmcMetadataModel::fetchMetadata()
{
    d->metadataUpdater->fetchMetadata(d->rowsToFetchMetadataFor);
    d->rowsToFetchMetadataFor.clear();
}

QList< int > PmcMetadataModel::rolesNeeded() const
{
    return QList<int>() << Qt::DisplayRole
                        << MediaCenter::ResourceIdRole
                        << MediaCenter::MediaUrlRole
                        << MediaCenter::MediaTypeRole
                        << Qt::DecorationRole;
}

QString PmcMetadataModel::mimetypeForResource(const Nepomuk::Resource& resource) const
{
    return resource.property(Nepomuk::Vocabulary::NIE::mimeType()).toString();
}

int PmcMetadataModel::rowCount(const QModelIndex& parent) const
{
    return d->numberOfEntries;
}

void PmcMetadataModel::saveMetadata(int row, const QHash< int, QVariant >& values )
{
    if (row >= rowCount() || d->metadataValues.contains(row))
        return;

    d->metadataValues.insert(row, values);
    emit dataChanged(createIndex(row, 0), createIndex(row, 0));
}

void PmcMetadataModel::newEntries(int count)
{
    int existingCount = rowCount();
    beginInsertRows(QModelIndex(), existingCount, existingCount+count);
    d->numberOfEntries += count;
    endInsertRows();
}

void PmcMetadataModel::finishedListing()
{
    qobject_cast<Nepomuk::Query::QueryServiceClient*>(sender())->close();
}

void PmcMetadataModel::error(const QString &message)
{
    kDebug() << message;
}

QString PmcMetadataModel::urlForResource(const Nepomuk::Resource &resource) const
{
    return resource.property(Nepomuk::Vocabulary::NIE::url()).toString();
}

QString PmcMetadataModel::fetchPreview(const KUrl &url, const QModelIndex& index)
{
    const QString prettyUrl = url.prettyUrl();
    if (PmcImageCache::instance()->containsId(prettyUrl)) {
        return "image://" + QString(PmcImageProvider::identificationString) + "/" + prettyUrl;
    }

    d->filesToPreview.insert(url, QPersistentModelIndex(index));
    d->previewTimer.start(100);
    return QString();
}

void PmcMetadataModel::delayedPreview()
{
    QHash<KUrl, QPersistentModelIndex>::const_iterator i = d->filesToPreview.constBegin();

    KFileItemList list;

    while (i != d->filesToPreview.constEnd()) {
        KUrl file = i.key();
        QPersistentModelIndex index = i.value();

        if (!d->previewJobs.contains(file) && file.isValid()) {
            list.append(KFileItem(file, QString(), 0));
            d->previewJobs.insert(file, QPersistentModelIndex(index));
        }
        ++i;
    }

    if (list.size() > 0) {
        KIO::PreviewJob* job = KIO::filePreview(list, d->thumbnailSize, d->thumbnailerPlugins);
        connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(showPreview(KFileItem,QPixmap)));
        connect(job, SIGNAL(failed(KFileItem)), SLOT(previewFailed(KFileItem)));
    }

    d->filesToPreview.clear();
}

void PmcMetadataModel::showPreview(const KFileItem &item, const QPixmap &preview)
{
    QPersistentModelIndex index = d->previewJobs.value(item.url());
    d->previewJobs.remove(item.url());

    if (index.isValid()) {
        PmcImageCache::instance()->addImage(item.url().prettyUrl(), preview.toImage());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::previewFailed(const KFileItem &item)
{
    QPersistentModelIndex index = d->previewJobs.value(item.url());
    d->previewJobs.remove(item.url());
    if (index.isValid()) {
        d->mediaUrlWhichFailedThumbnailGeneration.append(item.url().prettyUrl());
        emit dataChanged(index, index);
    }
}

void PmcMetadataModel::addFilter(const Nepomuk::Types::Property& property, const Nepomuk::Query::Term& term, Nepomuk::Query::ComparisonTerm::Comparator comparator)
{
    QList<Nepomuk::Query::Term> termsList;

    termsList.append(d->term);
    termsList.append(Nepomuk::Query::ComparisonTerm(property, term, comparator));

    d->term = Nepomuk::Query::AndTerm(termsList);
    d->updateTimer.start(100);
}

void PmcMetadataModel::clearAllFilters()
{
    d->term = d->resourceTypeTerm;
    d->updateTimer.start(100);
}

void PmcMetadataModel::handleUpdaterReset()
{
    resetModel();
}

void PmcMetadataModel::resetModel()
{
    beginResetModel();
    d->numberOfEntries = 0;
    d->metadataValues.clear();
    endResetModel();
}

void PmcMetadataModel::setSearchTerm(const QString& searchTerm)
{
    if (searchTerm.isEmpty()) {
        d->isSearchTermValid = false;
    } else {
        d->searchTerm = Nepomuk::Query::QueryParser::parseQuery(searchTerm).term();
        d->isSearchTermValid = true;
    }
    d->updateTimer.start(100);
}

#include "pmcmetadatamodel.moc"
