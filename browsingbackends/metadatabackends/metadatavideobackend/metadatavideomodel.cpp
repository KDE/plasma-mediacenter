/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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


#include "metadatavideomodel.h"
#include "metadatavideothumbnailprovider.h"

#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <KUrl>

#include <QtGui/QIcon>
#include <QtCore/QEvent>
#include <QtDeclarative/QDeclarativeEngine>

class MetadataVideoModel::Private
{
public:
    Private()
        : thumbProvider(new MetadataVideoThumbnailProvider(QDeclarativeImageProvider::Pixmap)) {
    }
    MetadataVideoThumbnailProvider* thumbProvider;
};

MetadataVideoModel::MetadataVideoModel(QObject* parent)
    : AbstractMetadataModel(parent)
    , d(new Private())
{
    if(metadataModel()) {
        metadataModel()->setProperty("resourceType", "nfo:FileDataObject");
        metadataModel()->setProperty("mimeType", "video");
        metadataModel()->setProperty("limit", 500);
    } else {
        kDebug() << "WARNING: Constructor called before metadataModel set :/";
    }

    connect(d->thumbProvider, SIGNAL(gotThumbnail(QString)), SLOT(gotThumbnail(QString)));
    qobject_cast<MediaCenter::AbstractBrowsingBackend*>(parent)->declarativeEngine()->addImageProvider(QLatin1String("metadatavideothumb"),
                                                                                                       d->thumbProvider);
}

MetadataVideoModel::~MetadataVideoModel()
{
    delete d;
}

void MetadataVideoModel::gotThumbnail(const QString& url)
{
    for(int i = 0; i < rowCount(); ++i) {
        if(data(index(i, 0), MediaCenter::MediaUrlRole).toUrl() == QUrl(url)) {
            emit dataChanged(index(i, 0), index(i, 0));
        }
    }
}

QVariant MetadataVideoModel::data(const QModelIndex& index, int role) const
{
    if(!metadataModel()) {
        return QVariant();
    }

    switch (role) {
    case MediaCenter::MediaTypeRole:
        return "video";
    case Qt::DecorationRole: {
        QString url = data(index, MediaCenter::MediaUrlRole).toUrl().toString();
        if (d->thumbProvider->hasThumbnail(url)) {
            return QString("image://metadatavideothumb/") + url;
        }
        d->thumbProvider->loadThumbnail(KUrl(url), QSize(256, 256));
    }
    default:
        return AbstractMetadataModel::data(index, role);
    }
}

#include "metadatavideomodel.moc"
