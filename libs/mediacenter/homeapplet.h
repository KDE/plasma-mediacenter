/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef HOMEAPPLET_H
#define HOMEAPPLET_H

#include <Plasma/Applet>

#include "mediacenter_export.h"

namespace MediaCenter {
class AbstractBrowsingBackend;

/**
 * @class HomeApplet
 * @author Alessandro Diaferia
 * @brief This is an abstract class for every Home Applet in Plasma Media Center.
 *
 * The Home Applet is used by Plasma Media Center to present welcome contents
 * to the user and some common home actions like Quit, Preferences..
 * In the Home Applet are shown the available browsing backends for the Browser applet.
 * This way the user can choose which media content to manage.
 * It is recommended to reimplement the init function and make it
 * show the available backends fetched through the availableBackends method
 * from the AbstractBrowsingBackend class.
 */
class MEDIACENTER_EXPORT HomeApplet : public Plasma::Applet
{
    Q_OBJECT
public:
    HomeApplet(QObject *parent, const QVariantList &args);
    virtual ~HomeApplet();

signals:
    /**
     * When one of the available backends is activated the applet
     * should create the respective instance of AbstractBrowsingBackend
     * and emit this signal in order for the browser applet to properly
     * load it.
     */
    void browsingBackendActivated(MediaCenter::AbstractBrowsingBackend *backend);

    void quitRequest();
    void showSettingsRequest();

private:
    class HomeAppletPrivate;
    HomeAppletPrivate *d;

};
} // namespace MediaCenter

#endif // HOMEAPPLET_H
