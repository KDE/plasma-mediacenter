/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>          `                     *
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

#include "baloosearchmediasource.h"

#include <mediacenter/medialibrary.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/mediacenter.h>
#include <baloo/query.h>
#include <baloo/result.h>


MEDIACENTER_EXPORT_MEDIASOURCE(BalooSearchMediaSource)

BalooSearchMediaSource::BalooSearchMediaSource(QObject* parent, const QVariantList& args)
    : AbstractMediaSource(parent, args)
{
}

BalooSearchMediaSource::~BalooSearchMediaSource()
{
}

void BalooSearchMediaSource::run()
{
    queryForMediaType("Audio");
    queryForMediaType("Image");
    queryForMediaType("Video");
}

void BalooSearchMediaSource::queryForMediaType(const QString& type)
{
    Baloo::Query query;
    query.addType(type);

    Baloo::ResultIterator it = query.exec();
    while (it.next()) {
        QHash<int, QVariant> values;
        values.insert(MediaCenter::MediaUrlRole, it.url());
        values.insert(MediaCenter::MediaTypeRole, type.toLower());
        values.insert(Qt::DisplayRole, it.text());
        values.insert(Qt::DecorationRole, it.icon());
        SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
    }
}

#include "baloosearchmediasource.moc"
