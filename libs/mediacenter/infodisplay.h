/***************************************************************************
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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
#ifndef INFODISPLAY_H
#define INFODISPLAY_H

#include "mediacenter_export.h"
#include "mediacenter.h"

#include <Plasma/Applet>
#include <Plasma/Slider>

namespace MediaCenter {

/**
 * @class InfoDisplay infodisplay.h
 * @brief Base class for display of media related information
 *
 * This is the base class to use when implementing objects that display information
 * for the Plasma MediaCenter Components. This object will initially be implemented
 * as a panel like widget at the bottom of the screen.
 */

class MEDIACENTER_EXPORT InfoDisplay : public Plasma::Applet
{
    Q_OBJECT
public:
    InfoDisplay(QObject *parent, const QVariantList &args);
    virtual ~InfoDisplay();

    /**
     * Reimplement this to be able to put widgets into the applet on each state changes.
     * Call applyLayouts when finished adding components to the layout.
     */
    virtual void addToLayout(QGraphicsWidget *widget, const MediaCenter::LayoutZone &zone) = 0;
    virtual void setMode(const MediaCenter::InfoDisplayMode &mode) = 0;

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
    class InfoDisplayPrivate;
    InfoDisplayPrivate *d;
};

} // namespace MediaCenter

#endif // INFODISPLAY_H
