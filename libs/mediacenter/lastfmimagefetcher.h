/***********************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef LASTFMIMAGEFETCHER_H
#define LASTFMIMAGEFETCHER_H

#include <QObject>
#include <QQueue>
#include <QHash>
#include <QNetworkAccessManager>
#include <QVariant>

#include "mediacenter_export.h"

class LastFmImageFetcher : public QObject
{
    Q_OBJECT
public:
    explicit LastFmImageFetcher(QObject* parent = 0);
    ~LastFmImageFetcher();

    void fetchImage(const QString& type, const QVariant& identifier, const QString& artistName, const QString& albumName = QString());

Q_SIGNALS:
    void imageFetched(const QVariant &identifier, const QString &artistName);

private Q_SLOTS:
    void processQueue();
    void gotResponse(QNetworkReply* reply);
    void gotImage(QNetworkReply* reply);

private:
    void downloadImage(const QString& type, const QString& name, const QString& url);

    bool m_busy;
    QString m_artistInfoUrl;
    QString m_albumInfoUrl;
    QQueue< QList<QString> > m_pendingQueue;
    QNetworkAccessManager m_netAccessManager;
    QNetworkAccessManager m_imageDownloadManager;
    QHash<QNetworkReply*, QString> m_currentInfoDownloads;
    QHash<QNetworkReply*, QPair<QString,QString> > m_currentImageDownloads;
    QHash<QString, QVariant> m_identifiers;
};

#endif // LASTFMIMAGEFETCHER_H_H
