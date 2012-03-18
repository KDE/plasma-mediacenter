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
#include "utils.h"

#include <Plasma/DataEngine>
#include <Plasma/DataEngineManager>
#include <Plasma/Theme>
#include <Plasma/Svg>

#include <QPainter>
#include <QPainterPath>
#include <QRect>

#include <KDebug>

namespace MediaCenter {
    Plasma::DataEngine *loadEngineOnce(const QString &name)
    {
        Plasma::DataEngine *engine = Plasma::DataEngineManager::self()->engine(name);
        if (!engine->isValid()) {
            engine = Plasma::DataEngineManager::self()->loadEngine(name);
            if (!engine->isValid()) {
                kWarning() << "unable to load" << name << "engine";
            }
        }
	return engine;
    }

    void drawCloseEmblem(QPainter *painter, const QRect &rect)
    {
        Plasma::Svg svg;
        svg.setImagePath("widgets/configuration-icons");
        svg.paint(painter, rect, "close");
    }

    void drawReloadEmblem(QPainter *painter, const QRect &rect)
    {
        Plasma::Svg svg;
        svg.setImagePath("widgets/configuration-icons");
        svg.paint(painter, rect, "rotate");
    }
}
