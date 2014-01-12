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

#include "pmcmetadatamodel.h"
#include "pmcimagecache.h"
#include "pmcimageprovider.h"
#include "metadataupdater.h"
#include "lastfmimagefetcher.h"

#include <Nepomuk2/Query/Query>
#include <Nepomuk2/Vocabulary/NIE>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Query/ResourceTypeTerm>
#include <Nepomuk2/Query/AndTerm>
#include <Nepomuk2/Query/QueryServiceClient>
#include <Nepomuk2/Query/Result>
#include <Nepomuk2/Variant>
#include <Nepomuk2/Query/QueryParser>

#include <KIO/PreviewJob>
#include <KDebug>

#include <QtCore/QTimer>

class PmcMetadataModel::Private
{
public:
    Private()
        : thumbnailerPlugins(new QStringList(KIO::PreviewJob::availablePlugins()))
        , isSearchTermValid(false)
        , numberOfEntries(0)
    {
    }
    Nepomuk2::Query::Term term;
    Nepomuk2::Query::Term searchTerm;
    bool isSearchTermValid;
    QList< Nepomuk2::Query::Result > queryResults;

    //Thumbnail stuff
    const QStringList *thumbnailerPlugins;
    QHash<KUrl, QPersistentModelIndex> filesToPreview;
    QSize thumbnailSize;
    QHash<KUrl, QPersistentModelIndex> previewJobs;
    QTimer previewTimer;
    QTimer updateTimer;
    QTimer metadataFetchTimer;
    Nepomuk2::Query::ResourceTypeTerm resourceTypeTerm;
    QHash<int, QHash<int, QVariant> > metadataValues;
    MetadataUpdater *metadataUpdater;
    int numberOfEntries;
    QList<int> rowsToFetchMetadataFor;
    QStringList mediaUrlWhichFailedThumbnailGeneration;
    QVariant defaultDecoration;
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
    connect(d->metadataUpdater, SIGNAL(gotMetadata(int,QHash<int,QVariant>)),
            SLOT(saveMetadata(int,QHash<int,QVariant>)));
    connect(d->metadataUpdater, SIGNAL(newResults(int)), SLOT(newEntries(int)));
    connect(d->metadataUpdater, SIGNAL(reset()), SLOT(handleUpdaterReset()));
    connect(d->metadataUpdater, SIGNAL(queryStarted()), SIGNAL(queryStarted()));
    connect(d->metadataUpdater, SIGNAL(queryFinished()), SIGNAL(queryFinished()));
    connect(d->metadataUpdater, SIGNAL(queryError(QString)), SIGNAL(queryError(QString)));
    d->metadataUpdater->start(QThread::IdlePriority);

    connect(LastFmImageFetcher::instance(), SIGNAL(imageFetched(QPersistentModelIndex,QString)), SLOT(signalUpdate(QPersistentModelIndex,QString)));
}

PmcMetadataModel::~PmcMetadataModel()
{
    d->metadataUpdater->quit();
    kDebug() << "Waiting for metadata thread to quit...";
    d->metadataUpdater->wait(5000);
    delete d;
}

void PmcMetadataModel::updateModel()
{
    QList<Nepomuk2::Query::Term> listOfTerms;
    listOfTerms.append(d->term);

    if (d->resourceTypeTerm.isValid()) {
        Nepomuk2::Query::ComparisonTerm sortTerm(Nepomuk2::Vocabulary::NIE::lastModified(), Nepomuk2::Query::Term());
        sortTerm.setSortWeight(1, Qt::DescendingOrder);
        listOfTerms.append(sortTerm);
    }

    if (d->isSearchTermValid) {
        listOfTerms.append(d->searchTerm);
    }

    d->metadataUpdater->setTerm(Nepomuk2::Query::AndTerm(listOfTerms));
    emit queryStarted();
}

void PmcMetadataModel::showMediaType(MediaCenter::MediaType mediaType)
{
    switch (mediaType) {
        case MediaCenter::Music:
            d->resourceTypeTerm = Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Audio());
	    break;
        case MediaCenter::Picture:
            d->resourceTypeTerm = Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Image());
	    break;
        case MediaCenter::Video:
            d->resourceTypeTerm = Nepomuk2::Query::ResourceTypeTerm(Nepomuk2::Vocabulary::NFO::Video());
    }
    clearAllFilters();
    d->updateTimer.start(100);
}

void PmcMetadataModel::showMediaForProperty(Nepomuk2::Types::Property property)
{
    Nepomuk2::Query::ComparisonTerm ct(property, Nepomuk2::Query::Term());
    ct.setInverted(true);
    setTerm(ct);
}

void PmcMetadataModel::setTerm(const Nepomuk2::Query::Term& term)
{
     d->term = term;
     d->updateTimer.start(100);
}

void PmcMetadataModel::addTerm(const Nepomuk2::Query::Term& term)
{
    d->term = Nepomuk2::Query::AndTerm(d->term, term);
    d->updateTimer.start(100);
}

QVariant PmcMetadataModel::metadataValueForRole(const QModelIndex& index, int role) const
{
    return d->metadataValues.value(index.row()).value(role);
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

    const QVariant metadataValue = metadataValueForRole(index, role);
    switch(role) {
    case MediaCenter::ResourceIdRole:
    case MediaCenter::MediaUrlRole:
    case MediaCenter::MediaTypeRole:
    case Qt::DisplayRole:
        return metadataValue;
    case Qt::DecorationRole:
        return decorationForMetadata(metadataValue, index);
    case MediaCenter::MediaThumbnailRole:
        if (metadataValueForRole(index, MediaCenter::MediaTypeRole) == "video") {
            KUrl url = d->metadataValues.value(index.row()).value(MediaCenter::MediaUrlRole).toUrl();
            if (d->mediaUrlWhichFailedThumbnailGeneration.contains(url.prettyUrl()))
                return "image-missing";
            return const_cast<PmcMetadataModel*>(this)->fetchPreview(url, index);
        }
    }

    return QVariant();
}

QVariant PmcMetadataModel::decorationForMetadata(const QVariant &metadataValue, const QModelIndex &index) const
{
    if (metadataValue.type() == QVariant::String && metadataValue.toString().isEmpty()) {
        const QString mediaType = metadataValueForRole(index, MediaCenter::MediaTypeRole).toString();
        if (mediaType == "album") {
            const QString albumName = metadataValueForRole(index, Qt::DisplayRole).toString();
            const QString albumUri = QString("album:%1").arg(albumName);
            if (PmcImageCache::instance()->containsId(albumUri)) {
                return QString("image://%1/%2").arg(PmcImageProvider::identificationString, albumUri);
            } else {
                //MediaUrlRole is abused for albums to pass on the artist name
                const QString artistNameForAlbum = metadataValueForRole(index, MediaCenter::MediaUrlRole).toString();
                LastFmImageFetcher::instance()->fetchImage("album", QPersistentModelIndex(index), artistNameForAlbum, albumName);
            }
        } else if (mediaType == "artist") {
            const QString artistName = metadataValueForRole(index, Qt::DisplayRole).toString();
            const QString artistUri = QString("artist:%1").arg(artistName);
            if (PmcImageCache::instance()->containsId(artistUri)) {
                return QString("image://%1/%2").arg(PmcImageProvider::identificationString, artistUri);
            } else {
                LastFmImageFetcher::instance()->fetchImage("artist", QPersistentModelIndex(index), artistName);
            }
        }
        if (d->defaultDecoration.isValid()) {
            return d->defaultDecoration;
        }
    }
    return metadataValue;
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

QString PmcMetadataModel::mimetypeForResource(const Nepomuk2::Resource& resource) const
{
    return resource.property(Nepomuk2::Vocabulary::NIE::mimeType()).toString();
}

int PmcMetadataModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
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
    qobject_cast<Nepomuk2::Query::QueryServiceClient*>(sender())->close();
}

QString PmcMetadataModel::urlForResource(const Nepomuk2::Resource &resource) const
{
    return resource.property(Nepomuk2::Vocabulary::NIE::url()).toString();
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

void PmcMetadataModel::addFilter(const Nepomuk2::Types::Property& property, const Nepomuk2::Query::Term& term, Nepomuk2::Query::ComparisonTerm::Comparator comparator)
{
    QList<Nepomuk2::Query::Term> termsList;

    termsList.append(d->term);
    termsList.append(Nepomuk2::Query::ComparisonTerm(property, term, comparator));

    d->term = Nepomuk2::Query::AndTerm(termsList);
    d->updateTimer.start(100);
}

void PmcMetadataModel::clearAllFilters()
{
    if (d->term == d->resourceTypeTerm)
        return;
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
        d->searchTerm = Nepomuk2::Query::QueryParser::parseQuery(searchTerm).term();
        d->isSearchTermValid = true;
    }
    d->updateTimer.start(100);
}

void PmcMetadataModel::setDefaultDecoration ( const QVariant& decoration )
{
    d->defaultDecoration = decoration;
}

void PmcMetadataModel::signalUpdate(const QPersistentModelIndex& index, const QString& displayString)
{
    if (!displayString.isEmpty() &&
        metadataValueForRole(index, Qt::DisplayRole).toString() != displayString) {
        return;
    }

    emit dataChanged(index, index);
}

#include "pmcmetadatamodel.moc"
