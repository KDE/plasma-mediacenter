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

#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <KDebug>
#include <KDE/KUrl>

#include <QtGui/QIcon>
#include <QtCore/QEvent>
#include <QtDeclarative/QDeclarativeEngine>

class MetadataVideoModel::Private
{
public:
    int thumbnailKey;
};

MetadataVideoModel::MetadataVideoModel(QObject* parent)
    : AbstractMetadataModel(parent)
    , d(new Private())
{
    if(metadataModel()) {
        d->thumbnailKey = metadataModel()->roleNames().key("thumbnail");
        resetMetadataModel();
        metadataModel()->setProperty("thumbnailSize", QSize(600, 600));
        metadataModel()->setProperty("resourceType", "nfo:FileDataObject");
        metadataModel()->setProperty("mimeType", "video");
    } else {
        kDebug() << "WARNING: Constructor called before metadataModel set :/";
    }
}

MetadataVideoModel::~MetadataVideoModel()
{
    delete d;
}

QVariant MetadataVideoModel::data(const QModelIndex& index, int role) const
{
    if(!metadataModel()) {
        return QVariant();
    }

    switch (role) {
    case MediaCenter::MediaTypeRole:
        return "video";
    case Qt::DecorationRole:
        return AbstractMetadataModel::data(index, d->thumbnailKey);
    case MediaCenter::DecorationTypeRole:
        return "qimage";
    case MediaCenter::HideLabelRole:
        return false;
    default:
        return AbstractMetadataModel::data(index, role);
    }
}

#include "metadatavideomodel.moc"
