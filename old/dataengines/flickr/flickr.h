/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
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
#ifndef FLICKRENGINE_H
#define FLICKRENGINE_H

#include <Plasma/DataEngine>

class FlickrInterface;
/**
 * @class FlickrEngine
 * @brief An engine that manages queries to Flickr.
 *
 * The engine returns search results as sources.
 * each source as a number of keys equal to the number of
 * search results from the query. The ids correspond to the source keys.
 * For each key a Plasma::DataEngine::Data is
 * associated. See FlickrInterface in order to see the keys available to
 * retrieve informations.
 *
 * @author Onur-Hayri Bakici
 */
class FlickrEngine : public Plasma::DataEngine
{
    Q_OBJECT
public:
    FlickrEngine(QObject *parent, const QVariantList &args);
    ~FlickrEngine();

protected:
    bool sourceRequestEvent(const QString &source);

protected slots:
    void slotSourceFromResult(const QString &searchTerm, const Plasma::DataEngine::Data &image);

private:
    FlickrInterface *m_interface;
};

#endif //FLICKRENGINE_H
