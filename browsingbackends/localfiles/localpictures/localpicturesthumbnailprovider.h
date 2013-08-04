/***************************************************************************
 *   Copyright 2011 Shantanu Tushar <shantanu@kde.org>                     *
 *   Copyright 2013 Akshay Ratan <akshayratan@gmail.com>                   *
 *
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

#ifndef PICTURESTHUMBNAILPROVIDER_H
#define PICTURESTHUMBNAILPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

#include <KUrl>

namespace KIO
{
class PreviewJob;
};

class KFileItemList;
class KFileItem;

class PicturesThumbnailProvider: public QObject, public QDeclarativeImageProvider
{
    Q_OBJECT
public:
    explicit PicturesThumbnailProvider(QObject* parent = 0);
    virtual ~PicturesThumbnailProvider();

    virtual QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);

    void loadThumbnails(const KUrl::List& fileList, const QSize& size);
    void loadThumbnail(const KUrl& file, const QSize& size);
    bool hasThumbnail(const QString& url);

signals:
    void gotThumbnail(const QString& url);

private slots:
    void processPreview(const KFileItem& file, const QPixmap& thumbnail);

private:
    class Private;
    Private* const d;
};

#endif // PICTURESTHUMBNAILPROVIDER_H
