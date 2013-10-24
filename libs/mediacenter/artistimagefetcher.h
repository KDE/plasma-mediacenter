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

#ifndef ARTISTIMAGEFETCHER_H
#define ARTISTIMAGEFETCHER_H

#include <QObject>
#include <QQueue>
#include <QHash>
#include <QNetworkAccessManager>

class ArtistImageFetcher : public QObject
{
    Q_OBJECT
public:
    class Singleton;

    static ArtistImageFetcher *instance();

    static const char *artistIdentification;
    explicit ArtistImageFetcher(QObject* parent = 0);
    ~ArtistImageFetcher();

    void fetchArtistImage(const QString &artistImage);

private Q_SLOTS:
    void processQueue();
    void gotResponse(QNetworkReply* reply);
    void gotImage(QNetworkReply* reply);

private:
    void downloadImage(const QString &artistName, const QString &url);

    bool m_busy;
    QString m_artistInfoUrl;
    QQueue<QString> m_artistQueue;
    QNetworkAccessManager m_netAccessManager;
    QNetworkAccessManager m_imageDownloadManager;
    QHash<QNetworkReply*, QString> m_currentArtistImageDownloads;
};

#endif // ARTISTIMAGEFETCHER_H
