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

/**
 * @class YouTubeInterface
 * @brief The interface that queries YouTube with search terms
 * @author Alessandro Diaferia
 *
 * This class interfaces YouTube and returns result entries
 * with the signal result.
 */

class YouTubeInterface : public QObject
{
    Q_OBJECT
public:
    YouTubeInterface(QObject *parent = 0);
    ~YouTubeInterface();

    void query(const QString &searchTerm);

signals:
    /**
     * @return the video entry as Plasma::DataEngine::Data.
     * each value can be retrieved using the following keys:
     * "title" for the video title
     * "description" for the video description
     * "keywords" for the video keywords
     *
     * @param searchTerm is the search term to be used as source name.
     * @param id is the video id to be used as source key
     * @param video is the video result entry retrieved after the search.
     */
    void result(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &video);

protected slots:
    void ytDataReady(KIO::Job *job, const QByteArray &data);
    void parseResults(KJob *job);

private:
    QHash<KIO::Job*, QString> m_queries;
    QHash<KIO::Job*, QString> m_datas;
};

#endif
