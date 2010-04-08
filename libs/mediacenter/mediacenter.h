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
#ifndef MEDIACENTER_H
#define MEDIACENTER_H

#include "mediacenter_export.h"

#include <QPair>

namespace Phonon {
    class MediaSource;
}

namespace MediaCenter {

enum MediaType {
    Invalid = 0,
    Picture,
    Video,
    Audio,
    OpticalDisc
};

enum UIComponent {
    UIControlBar,
    UIInfoBar,
    UIBrowser,
    UIPlayer,
    UIPlaylist,
    UIHomeScreen
};

enum PlaybackState {
    PlayingState,
    StoppedState, //TODO: Remove stopped state
    PausedState
};

/**
 * @typedef Media defines a QPair of MediaCenter::MediaType and QString.
 * The first argument is the type of the media while the second is a QString
 * defining the media. It might be both an udi string pointing to the device
 * hosting the media or the path to a media file.
 */
typedef QPair<MediaType, QString> Media;

/**
 * Returns the correct type identifying the source represented
 * by the given string.
 *
 * @param media: the string representing the media. It could be
 *               either the udi of the device hosting the media
 *               or the path of the local file.
 */
MEDIACENTER_EXPORT MediaType getType(const QString &media);

/**
 * @return a MediaCenter::Media from the given Phonon::MediaSource or
 * an invalid MediaCenter::Media if an error occurrs.
 * @note an invalid MediaCenter::Media has MediaType Invalid and empty QString.
 */
MEDIACENTER_EXPORT Media mediaFromMediaSource(const Phonon::MediaSource &);

} // namespace MediaCenter

#endif // MEDIACENTER_H
