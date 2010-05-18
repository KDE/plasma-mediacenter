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

#ifndef HOMESTATE_H
#define HOMESTATE_H

#include "mediacenterstate.h"

#include <Plasma/IconWidget>
#include <Plasma/TabBar>

namespace MediaCenter {

class MEDIACENTER_EXPORT HomeState : public MediaCenterState
{
    Q_OBJECT
public:
    HomeState(QState *parent = 0);
    virtual ~HomeState();

    virtual QList<QGraphicsWidget*> subComponents() const;
    virtual void configure();
    virtual void initConnections();

protected:
    virtual void onExit(QEvent* event);
    virtual void onEntry(QEvent* event);

private:
    Plasma::IconWidget *m_quit;

};

} //namespace end
#endif // HOMESTATE_H
