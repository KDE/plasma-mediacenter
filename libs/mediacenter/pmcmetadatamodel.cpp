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

#include <KIO/PreviewJob>
#include <KDebug>

#include <QtCore/QTimer>

class PmcMetadataModel::Private
{
public:
    Private()
        : thumbnailerPlugins(new QStringList(KIO::PreviewJob::availablePlugins()))
    {
    }
    Nepomuk::Query::Term term;
    QList< Nepomuk::Query::Result > queryResults;

    //Thumbnail stuff
    const QStringList *thumbnailerPlugins;
    QHash<KUrl, QPersistentModelIndex> filesToPreview;
    QSize thumbnailSize;
    QHash<KUrl, QPersistentModelIndex> previewJobs;
    QTimer previewTimer;
    QTimer updateTimer;
    Nepomuk::Query::ResourceTypeTerm resourceTypeTerm;
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

    d->thumbnailSize = QSize(512, 512);
}

PmcMetadataModel::~PmcMetadataModel()
{
    delete d;
}

void PmcMetadataModel::updateModel()
{
    Nepomuk::Query::Query myQuery;
    Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);

    connect(queryClient, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
            this, SLOT(newEntries(QList<Nepomuk::Query::Result>)));
    connect(queryClient, SIGNAL(entriesRemoved(QList<QUrl>)),SLOT(entriesRemoved(QList<QUrl>)));
    connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));

    myQuery.setTerm(d->term);
    kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();

    beginResetModel();
    d->queryResults.clear();
    endResetModel();

    queryClient->query(myQuery);
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
    d->term = ct;
    d->updateTimer.start(100);
}

void PmcMetadataModel::setTerm(const Nepomuk::Query::Term& term)
{
     d->term = term;
     d->updateTimer.start(100);
}

QVariant PmcMetadataModel::data(const QModelIndex& index, int role) const
{
    Nepomuk::Query::Result result = d->queryResults.at(index.row());
    switch(role) {
    case Qt::DecorationRole: {
        if (mimetypeForResource(result.resource()).startsWith("video")) {
            return const_cast<PmcMetadataModel*>(this)->fetchPreview(urlForResource(result.resource()), index);
        }
        const QString icon = result.resource().genericIcon();
        if (!icon.isEmpty()) return icon;
        return "kde"; }
    case Qt::DisplayRole:
        return result.resource().genericLabel();
    case MediaCenter::ResourceIdRole:
        return result.resource().resourceUri();
    case MediaCenter::MediaUrlRole:
        return urlForResource(result.resource());
    case MediaCenter::MediaThumbnailRole:
        return const_cast<PmcMetadataModel*>(this)->fetchPreview(urlForResource(result.resource()), index);
    case MediaCenter::MediaTypeRole:
        //TODO: Put null checks here
        return result.resource().property(Nepomuk::Vocabulary::NIE::mimeType()).toString().split("/").at(0);
    case MediaCenter::DecorationTypeRole: {
            return "image"; }
    }

    return QVariant();
}

QString PmcMetadataModel::mimetypeForResource(const Nepomuk::Resource& resource) const
{
    return resource.property(Nepomuk::Vocabulary::NIE::mimeType()).toString();
}

int PmcMetadataModel::rowCount(const QModelIndex& parent) const
{
    return d->queryResults.count();
}

void PmcMetadataModel::newEntries(const QList< Nepomuk::Query::Result >& entries)
{
    Q_FOREACH (const Nepomuk::Query::Result &res, entries) {
//         kDebug() << "RESOURCE " << res.resource().genericLabel();
//         Q_FOREACH (const QString &key, res.resource().allProperties().keys()) {
//             kDebug() << key << " = " << res.resource().allProperties().value(key);
//         }
        d->queryResults.append(res);
    }
}

void PmcMetadataModel::entriesRemoved(QList< QUrl > entries)
{
}

void PmcMetadataModel::finishedListing()
{
    beginResetModel();
    endResetModel();
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
    return "kde";
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
        connect(job, SIGNAL(failed(KFileItem)), SLOT(previewFailed()));
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
    //TODO: don't fail silently, let the view know
    d->previewJobs.remove(item.url());
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

#include "pmcmetadatamodel.moc"
