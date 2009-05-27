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
#ifndef YOUTUBEENGINE_H
#define YOUTUBEENGINE_H

#include <Plasma/DataEngine>

namespace Plasma {
    class Service;
}
class YouTubeInterface;
/**
 * @class YouTubeEngine
 * @brief An engine that manages queries to YouTube.
 *
 * The engine returns search results as sources.
 * each source as a number of keys equal to the number of
 * search results from the query. The video ids correspond to the source keys.
 * For each key a Plasma::DataEngine::Data is
 * associated. See YouTubeInterface in order to see the keys available to retrieve
 * single video informations.
 *
 * @author Alessandro Diaferia
 */
class YouTubeEngine : public Plasma::DataEngine
{
    Q_OBJECT
public:
    YouTubeEngine(QObject *parent, const QVariantList &args);
    ~YouTubeEngine();

protected:
    bool sourceRequestEvent(const QString &source);

protected slots:
    void slotSourceFromResult(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &video);

private:
    YouTubeInterface *m_interface;
};

#endif
