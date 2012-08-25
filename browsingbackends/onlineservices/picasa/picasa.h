/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
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
#ifndef PICASAENGINE_H
#define PICASAENGINE_H

#include <Plasma/DataEngine>

class PicasaInterface;
/**
 * @class PicasaEngine
 * @brief An engine that manages queries to Picasa.
 *
 * The engine returns search results as sources.
 * each source as a number of keys equal to the number of
 * search results from the query. The ids correspond to the source keys.
 * For each key a Plasma::DataEngine::Data is
 * associated. See PicasaInterface in order to see the keys available to
 * retrieve informations.
 *
 * @author Francesco Grieco
 */
class PicasaEngine : public Plasma::DataEngine
{
    Q_OBJECT
public:
    PicasaEngine(QObject *parent, const QVariantList &args);
    ~PicasaEngine();

protected:
    bool sourceRequestEvent(const QString &source);

protected slots:
    void slotSourceFromResult(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &video);

private:
    PicasaInterface *m_interface;
};

#endif
