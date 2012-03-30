/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2011  Shantanu Tushar shaan7in@gmail.com

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "localvideosmodel.h"

#include <libs/mediacenter/mediacenter.h>
#include <libs/mediacenter/abstractbrowsingbackend.h>

#include <KDirModel>

#include <QtDeclarative/QDeclarativeEngine>

LocalVideosModel::LocalVideosModel (QObject* parent)
    : LocalFilesAbstractModel (parent, QString("video/")),
    m_thumbProvider(new VideoThumbnailProvider(this))
{
    MediaCenter::AbstractBrowsingBackend *backend = qobject_cast<MediaCenter::AbstractBrowsingBackend*>(parent);
    backend->declarativeEngine()->addImageProvider("localvideothumbnail", m_thumbProvider);

    connect(m_thumbProvider, SIGNAL(gotThumbnail(QString)), SLOT(processThumbnail(QString)));
}

QVariant LocalVideosModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }

    if (role == Qt::DecorationRole && !data(index, MediaCenter::IsExpandableRole).toBool()) {
        const QString url = data(index, MediaCenter::MediaUrlRole).toString();

        if (m_thumbProvider->hasThumbnail(url)) {
            return "image://localvideothumbnail/" + url;
        } else {
            m_thumbProvider->loadThumbnail(KUrl(url), QSize(600, 600));
            m_pendingThumbs.insert(url, index);
        }
    }

    return LocalFilesAbstractModel::data (index, role);
}

void LocalVideosModel::processThumbnail (const QString &url)
{
    QModelIndex index = m_pendingThumbs.take(url);

    emit dataChanged(index, index);
}
