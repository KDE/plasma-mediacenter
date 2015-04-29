/*
 * Copyright 2015 Bhushan Shah <bshah@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "recentmediamodel.h"

#include <mediacenter/mediacenter.h>
#include <mediacenter/mediacenter.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/pmcmedia.h>
#include <mediacenter/pmccoverartprovider.h>

#include <QDebug>
#include <QUrl>

//TODO: Make pretty headers work out again
//#include <KActivitiesExperimentalStats/Cleaning>
//#include <KActivitiesExperimentalStats/ResultModel>
//#include <KActivitiesExperimentalStats/Terms>
#include <kactivitiesexperimentalstats/terms.h>
#include <kactivitiesexperimentalstats/resultmodel.h>
#include <kactivitiesexperimentalstats/resultset.h>

namespace KAStats = KActivities::Experimental::Stats;

using namespace KAStats;
using namespace KAStats::Terms;

class RecentMediaModel::Private
{
public:
    Private()
    {
    }
    ResultModel* result = nullptr;
    QString mediaType;
    MediaLibrary* mediaLibrary = nullptr;
};

RecentMediaModel::RecentMediaModel(QObject* parent) :
    QAbstractListModel(parent),
    d(new Private())
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    d->mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();
}

RecentMediaModel::~RecentMediaModel()
{
    if(d->result) {
        disconnect(d->result, 0, this, 0);
    }
    delete d;
}

int RecentMediaModel::rowCount(const QModelIndex& parent) const
{
    if(!d->result) {
        return 0;
    }
    return d->result->rowCount(parent);
}

void RecentMediaModel::setMediaType(const QString &type)
{
    d->mediaType = type;
}

void RecentMediaModel::query()
{
    auto query = UsedResources
        | RecentlyUsedFirst
        | Agent::any()
        | Type::Type(d->mediaType)
        | Activity::current()
        | Url::file();

    d->result = new ResultModel(query);

    connect(d->result, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
            this, SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SIGNAL(rowsAboutToBeMoved(QModelIndex,int,int,QModelIndex,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            this, SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutAboutToBeChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SIGNAL(rowsInserted(QModelIndex,int,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SIGNAL(countChanged()), Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            this, SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SIGNAL(countChanged()), Qt::UniqueConnection);
    connect(d->result, SIGNAL(modelAboutToBeReset()),
            this, SIGNAL(modelAboutToBeReset()),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(modelReset()),
            this, SIGNAL(modelReset()),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(modelReset()),
            this, SIGNAL(countChanged()),
            Qt::UniqueConnection);
    connect(d->result, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            this, SIGNAL(layoutChanged(QList<QPersistentModelIndex>,QAbstractItemModel::LayoutChangeHint)),
            Qt::UniqueConnection);
}

QVariant RecentMediaModel::data(const QModelIndex& index, int role) const
{
    const int row=index.row();

    if (!index.isValid() || row >= rowCount())
        return QVariant();

    const QUrl url = QUrl::fromLocalFile(d->result->data(index, ResultModel::ResourceRole).toString());
    auto media = d->mediaLibrary->mediaForUrl(url.toString());

    switch (role) {
    case MediaCenter::ResourceIdRole:
        return media->sha();
    case MediaCenter::MediaUrlRole:
        return media->url();
    case MediaCenter::MediaTypeRole:
        return media->type();
    case Qt::DisplayRole:
        return media->title();
    case MediaCenter::AlbumRole:
        return media->album();
    case MediaCenter::ArtistRole:
        return media->artist();
    case MediaCenter::RatingRole:
        return media->rating();
    case MediaCenter::DurationRole:
        return media->duration();
    case MediaCenter::GenreRole:
        return media->genre();
    case Qt::DecorationRole:
        if (media->type() == "video")
            return PmcCoverArtProvider::qmlImageUriForMediaFileCover(media->url());
        else if (media->type() == "audio")
            return PmcCoverArtProvider::qmlImageUriForAlbumCover(media->album());
        else
            return media->thumbnail();
    case MediaCenter::CreatedAtRole:
        return media->createdAt();
    case MediaCenter::HideLabelRole:
        return(media->type() == "image");
    case MediaCenter::IsExpandableRole:
        return false;
    }


    return QVariant();
}
