/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef MEDIALIBRARY_H
#define MEDIALIBRARY_H

#include "mediacenter_export.h"

#include <QHash>
#include <QThread>
#include <QSharedPointer>

#include "media.h"
#include "pmcmedia.h"

class MEDIACENTER_EXPORT MediaLibrary : public QThread
{
    Q_OBJECT
public:
    class Singleton;

    static MediaLibrary *instance();

    explicit MediaLibrary(QObject* parent = 0);
    ~MediaLibrary();

    virtual void run();

    void updateMedia(const QHash<int, QVariant> &data);
    void updateMedia(const QString &url, const QHash<int, QVariant> &data);
    QList<QSharedPointer <PmcMedia> > getMedia(const QString &type);

Q_SIGNALS:
    void newMedia(const QList< QSharedPointer<PmcMedia> > &media);

private Q_SLOTS:
    void processRemainingRequests();
    void initDb();
    void emitNewMediaWithMediaList();

private:
    class Private;
    Private * const d;

    bool areThereUpdateRequests();
    void processNextRequest();
    QPair<QString, QHash<int, QVariant> > takeRequest();
    void updateLibrary();
    bool mediaExists(const QString &first) const;
    void addMedia(const QSharedPointer< Media > &m);
    QSharedPointer<Media> mediaForSha(const QString &sha);
    void emitNewMedia();
};

#endif // MEDIALIBRARY_H
