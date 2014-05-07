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

#ifndef ABSTRACTMEDIASOURCE_H
#define ABSTRACTMEDIASOURCE_H

#include <QtCore/QThread>
#include <KDE/KService>

#include "mediacenter_export.h"

class MediaLibrary;

namespace MediaCenter {
class MEDIACENTER_EXPORT AbstractMediaSource : public QThread
{
    Q_OBJECT
public:
    explicit AbstractMediaSource(QObject* parent = 0, const QVariantList& = QVariantList());
    ~AbstractMediaSource();

    void setMediaLibrary(MediaLibrary *mediaLibrary);
    static KService::List availableMediaSourcePlugins();

protected:
    MediaLibrary *mediaLibrary() const;

private:
    class Private;
    Private * const d;
};
}

#define MEDIACENTER_EXPORT_MEDIASOURCE( c ) \
    K_PLUGIN_FACTORY( MediaSourceFactory, registerPlugin< c >(); )
#endif // ABSTRACTMEDIASOURCE_H
