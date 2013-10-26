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

}

QString YoutubeBackend::backendCategory() const
{
    return "video";
}

bool YoutubeBackend::expand(int row)
{
    m_videoDetailsModel->setVideoUrl(m_youtubeModel->videoUrl(row));
    m_videoDetailsModel->setVideoThumbnail(m_youtubeModel->videoThumbnail(row));
    m_videoDetailsModel->retriveRealUrl();
    if (m_expanded) {
        m_expanded = false;
    }
    return true;
}

bool YoutubeBackend::initImpl()
{
    m_youtubeModel = new YoutubeModel(this);
    ModelMetadata *metadata = new ModelMetadata(m_youtubeModel, this);
    metadata->setSupportsSearch(true);
    m_videoDetailsModel = new VideoDetailsModel(this);
    if (!(m_youtubeModel && m_videoDetailsModel)) {
        return false;
    }
    m_expanded = true;
    connect(m_videoDetailsModel, SIGNAL(gotRealUrl()), this, SLOT(realUrlFound()));
    setModel(metadata);
    return true;
}

bool YoutubeBackend::goOneLevelUp()
{
    if (!m_expanded) {
        m_expanded = true;
        setModel(m_youtubeModel);
        return true;
    }
    return false;
}

void YoutubeBackend::search(const QString& searchTerm)
{
    qobject_cast<YoutubeModel*>(model())->query(searchTerm);
}

void YoutubeBackend::realUrlFound()
{
        setModel(m_videoDetailsModel);
}
