/***************************************************************************
 *   Copyright 2013 by Sinny Kumari <ksinny@gmail.com>                     *
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

#include "youtubebackend.h"
#include "youtubemodel.h"
#include "videodetailsmodel.h"

#include "global.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(YoutubeBackend)

YoutubeBackend::YoutubeBackend(QObject* parent, const QVariantList& args): 
                               MediaCenter::AbstractBrowsingBackend(parent, args)
{
    youtubeModel = new YoutubeModel(this);
    videoDetailsModel = new VideoDetailsModel(this);
    m_expand = true;
    connect(videoDetailsModel, SIGNAL(gotRealUrl()), this, SLOT(realUrlFound()));
}

QString YoutubeBackend::backendCategory() const
{
    return "video";
}

bool YoutubeBackend::expand(int row)
{
    videoDetailsModel->setVideoUrl(youtubeModel->videoUrl(row));
    videoDetailsModel->setVideoThumbnail(youtubeModel->videoThumbnail(row));
    videoDetailsModel->retriveRealUrl();
    if (m_expand) {
        m_expand = false;
    }
    return true;
}

bool YoutubeBackend::initImpl()
{
    setModel(youtubeModel);
    return true;
}

bool YoutubeBackend::goOneLevelUp()
{
    if (!m_expand) {
        m_expand = true;
        setModel(youtubeModel);
        return true;
    } else {
        return false;
    }
}

bool YoutubeBackend::supportsSearch() const
{
    return true;
}

void YoutubeBackend::search(const QString& searchTerm)
{
    qobject_cast<YoutubeModel*>(model())->query(searchTerm);
}

void YoutubeBackend::realUrlFound()
{
        setModel(videoDetailsModel);
}
