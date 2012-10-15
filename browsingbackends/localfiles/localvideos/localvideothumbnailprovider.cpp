/***************************************************************************
 *   Copyright 2011 Shantanu Tushar <shantanu@kde.org>                     *
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

#include "localvideothumbnailprovider.h"

#include <KIO/PreviewJob>

class VideoThumbnailProvider::Private
{
public:
    Private() {
    }
    QHash<QString, QPixmap> pixmapCache;
};

VideoThumbnailProvider::VideoThumbnailProvider(QObject* parent)
    : QObject(parent)
    , QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
    , d(new Private())
{

}

VideoThumbnailProvider::~VideoThumbnailProvider()
{
    delete d;
}

void VideoThumbnailProvider::loadThumbnails(const KUrl::List& fileList, const QSize& size)
{
    KFileItemList fileItems;
    for(KUrl::List::ConstIterator it = fileList.begin(); it != fileList.end(); ++it) {
        if (!(*it).isValid())
            return;
        fileItems.append(KFileItem(KFileItem::Unknown, KFileItem::Unknown, *it, true));
    }

    QStringList* plugins = new QStringList;
    *plugins = KIO::PreviewJob::availablePlugins();

    KIO::PreviewJob* job = KIO::filePreview(fileItems, QSize(256, 256), plugins);
    connect(job, SIGNAL(gotPreview(KFileItem, QPixmap)), SLOT(processPreview(KFileItem, QPixmap)));
}

void VideoThumbnailProvider::loadThumbnail(const KUrl& file, const QSize& size)
{
    loadThumbnails(KUrl::List() << file, size);
}

void VideoThumbnailProvider::processPreview(const KFileItem& file, const QPixmap& thumbnail)
{
    d->pixmapCache.insert(file.url().prettyUrl(), thumbnail);
    emit gotThumbnail(file.url().prettyUrl());
}

QPixmap VideoThumbnailProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    if(d->pixmapCache.keys().contains(id)) {
        QPixmap pixmap = d->pixmapCache.value(id);
        if(size) {
            *size = pixmap.size();
        }
        if(requestedSize.isEmpty()) {
            return pixmap;
        } else {
            return pixmap.scaled(requestedSize, Qt::KeepAspectRatio);
        }
    } else {
        loadThumbnail(id, QSize(256, 256));
    }

    return QPixmap();
}

bool VideoThumbnailProvider::hasThumbnail(const QString& url)
{
    return d->pixmapCache.keys().contains(url);
}
