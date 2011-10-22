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
#ifndef PLAYBACKCONTROL_H
#define PLAYBACKCONTROL_H

#include "mediacenter_export.h"
#include "mediacenter.h"

#include <Plasma/Applet>
#include <Plasma/Slider>

namespace MediaCenter {

/**
 * @class PlaybackControl playbackcontrol.h
 * @brief Base class for controlling the media reproduction.
 *
 * This is the base class to use when implementing playback controls
 * for the Plasma MediaCenter Components. Keep in mind that most probably
 * this applet will be loaded and put on the topmost edge of the screen when
 * writing the painting code.
 *
 * Always use MediaCenter::MediaObject in order to retrieve current information
 * about media reproduction.
 */

class MEDIACENTER_EXPORT PlaybackControl : public Plasma::Applet
{
    Q_OBJECT
public:
    PlaybackControl(QObject *parent, const QVariantList &args);
    virtual ~PlaybackControl();

    /**
     * Reimplement this to be able to put widgets into the applet on each state changes.
     * Call applyLayouts when finished adding components to the layout.
     */
    virtual void addToLayout(QGraphicsWidget *component, const MediaCenter::LayoutZone &zone) = 0;

Q_SIGNALS:
    /**
     * Emit this signal in order to request the player to start a slideshow.
     */
    void mediaPlaySlideshow();

    void browserGoPrevious();
    void togglePlaylistVisible();
    void toggleControlAutohide();

public Q_SLOTS:
    /**
     * This slot should be reimplemented in order to invalidate layouts
     * and remove components from them. After this slot is called new
     * components can be added via the addToLayout method.
     * Call applyLayouts when finished adding components.
     */
    virtual void resetLayout() = 0;

    /**
     * Call this slot when the layout needs to be applied to the applet.
     */
    virtual void applyLayout() = 0;

private:
    class PlaybackControlPrivate;
    PlaybackControlPrivate *d;
};

} // namespace MediaCenter

#endif // PLAYBACKCONTROL_H
