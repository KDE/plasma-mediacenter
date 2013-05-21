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

class MediaInfoResult;
class PlaylistItem : public QObject
{
    Q_OBJECT
public:
    explicit PlaylistItem(const QString &url, QObject *parent);

    QString mediaUrl() const;
    QString mediaName() const;
    QString mediaArtist() const;
    int mediaLength() const;

Q_SIGNALS:
    void updated();

private Q_SLOTS:
    void update();
    void processMediaInfo(quint64 requestNumber, MediaInfoResult info);

private:
    mutable QTimer m_updateTimer;
    QString m_mediaUrl;
    QString m_mediaName;
    QString m_mediaArtist;
    int m_mediaLength;
    int m_serviceRequestNumber;
};

#endif // PLAYLISTITEM_H

class MediaInfoResult;
