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

#include "videodetailsmodel.h"
#include "video.h"

#include <libs/mediacenter/mediacenter.h>


VideoDetailsModel::VideoDetailsModel(QObject* parent): QAbstractListModel(parent)
{
    m_videoThumbnail = "http://i.ytimg.com/vi/gpMoGu81EEI/default.jpg";
    m_videoTitle = "KDE 4.10 beta 2";
    m_videoUrl = "http://www.youtube.com/watch?v=gpMoGu81EEI&feature=youtube_gdata_player";
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));realUrl();
}

VideoDetailsModel::~VideoDetailsModel()
{

}

int VideoDetailsModel::rowCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant VideoDetailsModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case MediaCenter::HideLabelRole:
            return false;
        case Qt::DecorationRole:
            return m_videoThumbnail;
        case MediaCenter::MediaUrlRole:
            return m_videoUrl;
        case Qt::DisplayRole:
            return m_videoTitle;
        case MediaCenter::MediaTypeRole:
            return "video";
        case MediaCenter::IsExpandableRole:
            return false;
    }
    return QVariant();
}

QString VideoDetailsModel::realUrl()
{
    Video *video = new Video;
    QUrl videoUrl(m_videoUrl);
    video->setWebpage(videoUrl);
    video->loadStreamUrl();
    connect(video, SIGNAL(gotStreamUrl(QUrl)), this, SLOT(streamUrl(QUrl)));
return  "";
}

void VideoDetailsModel::streamUrl(QUrl url)
{
    qDebug() << "URLLLLLLLLLLLLLL" << url.toString();
}
