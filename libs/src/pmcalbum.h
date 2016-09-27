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

#ifndef PMCALBUM_H
#define PMCALBUM_H

#include "mediacenter_export.h"
#include "media.h"

#include <QObject>
#include <QSharedPointer>

class Album;

class MEDIACENTER_EXPORT PmcAlbum : public QObject
{
    Q_OBJECT
public:
    explicit PmcAlbum(const QSharedPointer<Album> &album, QObject* parent = 0);
    ~PmcAlbum();

    const QString& name() const;
    QString albumArtist() const;
    int mediaCount() const;
    void removeMedia(const QSharedPointer<Media> &media);

signals:
    void updated();
    void removeRefs();

private:
    class Private;
    Private * const d;
};

#endif // PMCALBUM_H
