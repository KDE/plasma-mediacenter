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

/**
 * @class HomeApplet
 * @author Alessandro Diaferia
 * @brief This is an abstract class for every Home Applet in Plasma Media Center.
 *
 * The Home Applet is used by Plasma Media Center to present welcome contents
 * to the user and some common home actions like Quit, Preferences..
 * In the Home Applet are shown the available browsing backends for the Browser applet.
 * This way the user can choose which media content to manage.
 */
class HomeApplet : public Plasma::Applet
{
    Q_OBJECT
public:
    explicit HomeApplet(QObject *parent = 0);

signals:

public slots:

};

#endif // HOMEAPPLET_H
