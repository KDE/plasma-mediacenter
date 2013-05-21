/***********************************************************************************
 *   Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                          *
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

#ifndef MEDIAINFOSERVICE_H
#define MEDIAINFOSERVICE_H

#include <QtCore/QThread>

#include "mediainforesult.h"

class MediaInfoRequest;

class MediaInfoService : public QThread
{
    Q_OBJECT
public:
    explicit MediaInfoService(QObject* parent = 0);
    ~MediaInfoService();

    quint64 processRequest(MediaInfoRequest* request);

signals:
    void info(quint64 requestNumber, MediaInfoResult result);

protected:
    virtual void run();

private slots:
    void processPendingRequests();

private:
    bool areThereResultsToProcess() const;
    void fetchDataForRequest(quint64 requestNumber);
    quint64 nextRequestToProcess() const;

    class Private;
    Private * const d;
};

#endif // MEDIAINFOSERVICE_H
