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
        result = nullptr;
    }
    ResultModel* result;
    MediaLibrary* mediaLibrary;
};

RecentMediaModel::RecentMediaModel(QObject* parent):
    QAbstractListModel(parent),
    d(new Private())
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    d->mediaLibrary = SingletonFactory::instanceFor<MediaLibrary>();

    auto query = UsedResources
        | RecentlyUsedFirst
        | Agent::any()
        | Type::Type("video/*")
        | Activity::current()
        | Url::file();

    d->result = new ResultModel(query);
}

RecentMediaModel::~RecentMediaModel()
{
    delete d;
}

int RecentMediaModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return d->result->rowCount(parent);
}

QVariant RecentMediaModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)
    const int row=index.row();

    if (!index.isValid() || row >= rowCount())
        return QVariant();

    //const QString url = d->result->data(index, ResultModel::ResourceRole).toString();
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
        return "image://icon/image-missing";
    case MediaCenter::CreatedAtRole:
        return media->createdAt();
    case MediaCenter::HideLabelRole:
        return false;
    case MediaCenter::IsExpandableRole:
        return false;
    }


    return QVariant();
}
