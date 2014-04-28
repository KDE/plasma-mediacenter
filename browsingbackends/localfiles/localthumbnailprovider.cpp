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

#include "localthumbnailprovider.h"

#include <KIO/PreviewJob>

class ThumbnailProvider::Private
{
public:
    Private() {
    }
    QHash<QString, QPixmap> pixmapCache;
};

ThumbnailProvider::ThumbnailProvider(QObject* parent)
    : QObject(parent)
    , QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)
    , d(new Private())
{

}

ThumbnailProvider::~ThumbnailProvider()
{
    delete d;
}

void ThumbnailProvider::loadThumbnails(const QList<QUrl>& fileList, const QSize& size)
{
    //FIXME: Why is size unused?
    KFileItemList fileItems;
    for(QList<QUrl>::const_iterator it = fileList.begin(); it != fileList.end(); ++it) {
        if (!(*it).isValid())
            return;
        fileItems.append(KFileItem(KFileItem::Unknown, KFileItem::Unknown, *it, true));
    }

    QStringList* plugins = new QStringList;
    *plugins = KIO::PreviewJob::availablePlugins();

    KIO::PreviewJob* job = KIO::filePreview(fileItems, QSize(256, 256), plugins);
    connect(job, SIGNAL(gotPreview(KFileItem,QPixmap)), SLOT(processPreview(KFileItem,QPixmap)));
}

void ThumbnailProvider::loadThumbnail(const QUrl& file, const QSize& size)
{
    loadThumbnails(QList<QUrl>() << file, size);
}

void ThumbnailProvider::processPreview(const KFileItem& file, const QPixmap& thumbnail)
{
    d->pixmapCache.insert(file.url().url(), thumbnail);
    emit gotThumbnail(file.url().url());
}

QPixmap ThumbnailProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
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

bool ThumbnailProvider::hasThumbnail(const QString& url)
{
    return d->pixmapCache.keys().contains(url);
}
