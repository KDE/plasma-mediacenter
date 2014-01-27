/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#include "abstractmediasource.h"
#include <KServiceTypeTrader>
#include <KDebug>

using namespace MediaCenter;

AbstractMediaSource::AbstractMediaSource(QObject* parent, const QVariantList& args): QThread(parent)
{

}

AbstractMediaSource::~AbstractMediaSource()
{

}

KService::List AbstractMediaSource::availableMediaSourcePlugins()
{
    KService::List plugins = KServiceTypeTrader::self()->query("Plasma/MediaCenter/MediaSource");
    if (plugins.isEmpty()) {
        kWarning() << "no available media sources";
    }
    return plugins;
}
