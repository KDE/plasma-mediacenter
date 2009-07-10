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
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "mediacenter_export.h"

#include <Plasma/Applet>

namespace MediaCenter {

/**
 * @class Playlist playlist.h
 * This is the base class for the playlist applet in a media containment.
 * Use this as base class for implementing basic playlist handling.
 * Remember to use Playlist dataengine and eventually the cover fetcher dataengines
 * in order to retrieve useful data for the playlist items.
 */

class MEDIACENTER_EXPORT Playlist : public Plasma::Applet
{
    Q_OBJECT
public:
    Playlist(QObject *parent, const QVariantList &args);
    virtual ~Playlist();

signals:
    /**
     * Emit this signal whenever a new media gets added to the list.
     * Use the index to specify at which position the item has been dropped to.
     */
    void mediaAdded(int index, const QString &mediaString);

    /**
     * Use this convenience signal if the media has just been dropped at the end of
     * the playlist.
     */
    void mediaAppended(const QString &mediaString);

};

}

#endif // PLAYLIST_H
