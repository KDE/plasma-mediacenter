/***********************************************************************************
 *   Copyright Flavio Tordini <flavio.tordini@gmail.com>                           *
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

#ifndef NETWORKACCESS_H
#define NETWORKACCESS_H

#include <QTimer>
#include <QNetworkReply>

namespace The {
    QNetworkAccessManager* networkAccessManager();
}

class NetworkReply : public QObject {

    Q_OBJECT

public:
    explicit NetworkReply(QNetworkReply* networkReply);
    QNetworkReply* getNetworkReply() { return networkReply; }

signals:
    void data(QByteArray);
    void error(QNetworkReply*);
    void finished(QNetworkReply*);

private slots:
    void finished();
    void requestError(QNetworkReply::NetworkError);
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readTimeout();

private:
    void setupReply();
    QNetworkReply *networkReply;
    QTimer *readTimeoutTimer;
    int retryCount;

};

class NetworkAccess : public QObject {

    Q_OBJECT

public:
    explicit NetworkAccess(QObject* parent = 0);
    QNetworkReply* request(QUrl url,
                             int operation = QNetworkAccessManager::GetOperation,
                             const QByteArray &body = QByteArray());
    NetworkReply* get(QUrl url);
    NetworkReply* head(QUrl url);
    NetworkReply* post(QUrl url, const QMap<QString, QString>& params);

private:
    QNetworkRequest buildRequest(QUrl url);

};

#endif // NETWORKACCESS_H
