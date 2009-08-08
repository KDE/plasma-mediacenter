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
#ifndef BROWSINGWIDGET_H
#define BROWSINGWIDGET_H

#include <QGraphicsWidget>

namespace Plasma {
    class PushButton;
    class ComboBox;
}

class BrowsingWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum NavigationControl {
        NoControl = 0x0,
        BackwardControl = 0x1,
        ForwardControl = 0x2,
        UpLevelControl = 0x4,
        AllControls = 0x8
                     };
    Q_DECLARE_FLAGS(NavigationControls, NavigationControl);

    BrowsingWidget(QGraphicsItem *parent);
    ~BrowsingWidget();

    void setNavigationControls(NavigationControls controls);
    NavigationControls navigationControls();

    void setShowNavigationComboBox(bool set);
    bool showNavigationComboBox();

private:
    NavigationControls m_controls;
    bool m_navigationCombo;

    Plasma::PushButton *m_backwardButton;
    Plasma::PushButton *m_forwardButton;
    Plasma::PushButton *m_upLevelButton;
};

#endif // BROWSINGWIDGET_H
