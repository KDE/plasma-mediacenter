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

#include "networkaccess.h"
#include "constants.h"
#include <QtGui>

namespace The {
NetworkAccess* http();
}

/*
const QString USER_AGENT = QString(Constants::NAME)
                           + " " + Constants::VERSION
                           + " (" + Constants::WEBSITE + ")";
*/

const QString USER_AGENT = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_2) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.95 Safari/537.11";

NetworkReply::NetworkReply(QNetworkReply *networkReply) :
    QObject(networkReply),
    networkReply(networkReply),
    retryCount(0) {

    setupReply();

    readTimeoutTimer = new QTimer(this);
    readTimeoutTimer->setInterval(10000);
    readTimeoutTimer->setSingleShot(true);
    connect(readTimeoutTimer, SIGNAL(timeout()), SLOT(readTimeout()), Qt::UniqueConnection);
    readTimeoutTimer->start();
}

void NetworkReply::setupReply() {
    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)),
            SLOT(requestError(QNetworkReply::NetworkError)), Qt::UniqueConnection);
    connect(networkReply, SIGNAL(finished()),
            SLOT(finished()), Qt::UniqueConnection);
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)), Qt::UniqueConnection);
}

void NetworkReply::finished() {
    QUrl redirection = networkReply->attribute(
                QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirection.isValid()) {
        if (networkReply->operation() == QNetworkAccessManager::GetOperation
                || networkReply->operation() == QNetworkAccessManager::HeadOperation) {
            QNetworkReply *redirectReply =
                    The::http()->request(redirection, networkReply->operation());
            setParent(redirectReply);
            networkReply->deleteLater();
            networkReply = redirectReply;
            setupReply();
            readTimeoutTimer->start();
            return;
        } else qWarning() << "Redirection not supported" << networkReply->url().toEncoded();
    }

    if (receivers(SIGNAL(data(QByteArray))) > 0)
        emit data(networkReply->readAll());
    else if (receivers(SIGNAL(finished(QNetworkReply*))) > 0)
        emit finished(networkReply);

#ifndef QT_NO_DEBUG_OUTPUT
    if (!networkReply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool())
        qDebug() << networkReply->url().toEncoded();
#endif

    // bye bye my reply
    // this will also delete this NetworkReply as the QNetworkReply is its parent
    networkReply->deleteLater();
}

void NetworkReply::requestError(QNetworkReply::NetworkError code) {
    qDebug() << networkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
             << networkReply->errorString() << code;
    emit error(networkReply);
}

void NetworkReply::downloadProgress(qint64 bytesReceived, qint64 /* bytesTotal */) {
    // qDebug() << "Downloading" << bytesReceived << bytesTotal << networkReply->url();
    if (bytesReceived > 0 && readTimeoutTimer->isActive()) {
        readTimeoutTimer->stop();
        disconnect(networkReply, SIGNAL(downloadProgress(qint64,qint64)),
                   this, SLOT(downloadProgress(qint64,qint64)));
    }
}

void NetworkReply::readTimeout() {
    networkReply->disconnect();
    networkReply->abort();
    networkReply->deleteLater();

    if (networkReply->operation() != QNetworkAccessManager::GetOperation
            || networkReply->operation() != QNetworkAccessManager::HeadOperation) {
        emit error(networkReply);
        return;
    }

    if (retryCount > 3) {
        emit error(networkReply);
        return;
    }
    QNetworkReply *retryReply = The::http()->request(networkReply->url(), networkReply->operation());
    setParent(retryReply);
    networkReply = retryReply;
    setupReply();
    retryCount++;
    readTimeoutTimer->start();
}

/* --- NetworkAccess --- */

NetworkAccess::NetworkAccess( QObject* parent) : QObject( parent ) {}

QNetworkRequest NetworkAccess::buildRequest(QUrl url) {
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", USER_AGENT.toUtf8());
    request.setRawHeader("Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "en-us,en;q=0.5");
    request.setRawHeader("Connection", "Keep-Alive");
    return request;
}

QNetworkReply* NetworkAccess::request(QUrl url, int operation, const QByteArray& body) {
    QNetworkAccessManager *manager = The::networkAccessManager();

    QNetworkRequest request = buildRequest(url);

    QNetworkReply *networkReply;
    switch (operation) {

    case QNetworkAccessManager::GetOperation:
        networkReply = manager->get(request);
        break;

    case QNetworkAccessManager::HeadOperation:
        networkReply = manager->head(request);
        break;

    case QNetworkAccessManager::PostOperation:
        if (!body.isEmpty())
            request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
        networkReply = manager->post(request, body);
        break;

    default:
        qWarning() << "Unknown operation:" << operation;
        return 0;
    }

    return networkReply;
}

NetworkReply* NetworkAccess::get(const QUrl url) {
    QNetworkReply *networkReply = request(url);
    return new NetworkReply(networkReply);
}

NetworkReply* NetworkAccess::head(const QUrl url) {
    QNetworkReply *networkReply = request(url, QNetworkAccessManager::HeadOperation);
    return new NetworkReply(networkReply);
}

NetworkReply* NetworkAccess::post(const QUrl url, const QMap<QString, QString>& params) {
    QByteArray body;
    QMapIterator<QString, QString> i(params);
    while (i.hasNext()) {
        i.next();
        body += QUrl::toPercentEncoding(i.key())
                + '='
                + QUrl::toPercentEncoding(i.value())
                + '&';
    }
    QNetworkReply *networkReply = request(url, QNetworkAccessManager::PostOperation, body);
    return new NetworkReply(networkReply);
}
