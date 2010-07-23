/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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
#ifndef NAVIGATIONTOOLBAR_H
#define NAVIGATIONTOOLBAR_H

#include <QGraphicsWidget>
#include "mediacenter/mediacenter_export.h"

namespace Plasma {
    class ToolButton;
    class ComboBox;
    class TabBar;
}

namespace MediaCenter {

class MEDIACENTER_EXPORT NavigationToolbar : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum NavigationControl {
        NoControls = 0x0,
        BackwardControl = 0x1,
        ViewModeControl = 0x2,
        ForwardControl = 0x4,
        UpLevelControl = 0x8,
        AllControls = 0x15
    };
    Q_DECLARE_FLAGS(NavigationControls, NavigationControl)

    NavigationToolbar(QGraphicsItem *parent);
    ~NavigationToolbar();

    void setNavigationControls(NavigationControls controls);
    NavigationControls navigationControls();

    void setShowNavigationComboBox(bool set);
    bool showNavigationComboBox();

signals:
    void browsePath(const QString &path);
    void browseHistoryBack();
    void browseHistoryNext();
    void browseUp();

private:
    class NavigationToolbarPrivate;
    NavigationToolbarPrivate *d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(NavigationToolbar::NavigationControls)

} // MediaCenter namespace

#endif // BROWSINGWIDGET_H
