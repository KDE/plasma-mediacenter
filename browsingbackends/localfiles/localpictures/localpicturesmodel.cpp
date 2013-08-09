/*
    Copyright (C) 2011  Shantanu Tushar shantanu@kde.org
    Copyright (C) 2013 Akshay Ratan <akshayratan@gmail.com>

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


#include "localpicturesmodel.h"

#include <libs/mediacenter/mediacenter.h>
#include <libs/mediacenter/abstractbrowsingbackend.h>

#include <KDirModel>

#include <QtDeclarative/QDeclarativeEngine>

LocalPicturesModel::LocalPicturesModel (QObject* parent) : LocalFilesAbstractModel (parent, QString("image/")),  m_thumbProvider(new ThumbnailProvider(this))
{
    MediaCenter::AbstractBrowsingBackend *backend = qobject_cast<MediaCenter::AbstractBrowsingBackend*>(parent);
    backend->declarativeEngine()->addImageProvider("localpicturesthumbnail", m_thumbProvider);

    connect(m_thumbProvider, SIGNAL(gotThumbnail(QString)), SLOT(processThumbnail(QString)));
}

QVariant LocalPicturesModel::data (const QModelIndex& index, int role) const
{
    if(role == Qt::DecorationRole) {
        const QString url = data(index, MediaCenter::MediaUrlRole).toString();
	if (!LocalPicturesModel::data(index, MediaCenter::IsExpandableRole).toBool()) {
	    return LocalFilesAbstractModel::data (index, MediaCenter::MediaUrlRole);
	} else {
            if (m_thumbProvider->hasThumbnail(url)) {
	       return "image://localpicturesthumbnail/" + url;
	    } else {
                m_thumbProvider->loadThumbnail(KUrl(url), QSize(600, 600));
	        m_pendingThumbs.insert(url, index);
	    }
        }
    } else if(role == MediaCenter::HideLabelRole) {
            return !LocalPicturesModel::data(index, MediaCenter::IsExpandableRole).toBool();
        }
    return LocalFilesAbstractModel::data (index, role);
}

void LocalPicturesModel::processThumbnail (const QString &url)
{
    QModelIndex index = m_pendingThumbs.take(url);

    emit dataChanged(index, index);
}

