/***********************************************************************************
 *  Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                              *
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

#ifndef PLAYLISTITEM_H
#define PLAYLISTITEM_H

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QSharedPointer>

class PmcMedia;
class MediaInfoResult;

class PlaylistItem : public QObject
{
    Q_OBJECT
public:
    static const char *defaultArtist;
    static const int defaultLength;
    explicit PlaylistItem(const QString &url, QObject *parent);

    QString mediaUrl() const;
    QString mediaName() const;
    QString mediaArtist() const;
    int mediaLength() const;

Q_SIGNALS:
    void updated();

private:
    QSharedPointer<PmcMedia> m_media;
};

#endif // PLAYLISTITEM_H

class MediaInfoResult;
