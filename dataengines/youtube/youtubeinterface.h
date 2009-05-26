/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
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
#ifndef YOUTUBEINTERFACE_H
#define YOUTUBEINTERFACE_H

#include <QObject>
#include <Plasma/DataEngine>
#include <QHash>

namespace KIO {
    class Job;
}
class QByteArray;
class KJob;

class YouTubeInterface : public QObject
{
    Q_OBJECT
public:
    YouTubeInterface(QObject *parent = 0);
    ~YouTubeInterface();

    void query(const QString &searchTerm);

signals:
    void result(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &video);

protected slots:
    void ytDataReady(KIO::Job *job, const QByteArray &data);
    void parseResults(KJob *job);

private:
    QHash<KIO::Job*, QString> m_queries;
    QHash<KIO::Job*, QString> m_datas;
};

#endif
