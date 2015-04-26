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

#include "recentmediabackend.h"
#include "recentmediamodel.h"

#include <mediacenter/pmcmodel.h>

#include <KLocalizedString>

MEDIACENTER_EXPORT_BROWSINGBACKEND(RecentMediaBackend, "recentmediabackend.json")

RecentMediaBackend::RecentMediaBackend(QObject *parent, const QVariantList& args)
    : AbstractBrowsingBackend(parent, args)
{
}

RecentMediaBackend::~RecentMediaBackend()
{
}

bool RecentMediaBackend::initImpl()
{
    RecentMediaModel* videoModel = new RecentMediaModel(this);
    videoModel->setMediaType("video/*");
    videoModel->query();
    PmcModel* pmcVideoModel = new PmcModel(videoModel, this);
    pmcVideoModel->setName(i18n("Recent Videos"));
    setModel(pmcVideoModel);

    RecentMediaModel* musicModel = new RecentMediaModel(this);
    musicModel->setMediaType("audio/*");
    musicModel->query();
    PmcModel* pmcMusicModel = new PmcModel(musicModel, this);
    pmcMusicModel->setName(i18n("Recent Music"));
    addModel(pmcMusicModel);

    RecentMediaModel* imageModel = new RecentMediaModel(this);
    imageModel->setMediaType("image/*");
    imageModel->query();
    PmcModel* pmcImageModel = new PmcModel(imageModel, this);
    pmcImageModel->setName(i18n("Recent Pictures"));
    addModel(pmcImageModel);

    return true;
}

#include "recentmediabackend.moc"
