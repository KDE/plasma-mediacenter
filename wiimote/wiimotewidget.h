/*
 * Copyright 2010 Sebastian Kügler <sebas@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) version 3, or any
 * later version accepted by the membership of KDE e.V. (or its
 * successor approved by the membership of KDE e.V.), which shall
 * act as a proxy defined in Section 6 of version 3 of the license.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 */

#ifndef WIIMOTEWIDGET_H
#define WIIMOTEWIDGET_H

#include <QWidget>

#include "ui_wiimoteinterface.h"

class QLabel;
class KMainWindow;
class KPushButton;
class Wiimote;

class WiimoteWidget : public QWidget
{
    Q_OBJECT

    public:
        WiimoteWidget( KMainWindow *parent=0 );
        ~WiimoteWidget();

    public Q_SLOTS:
        void statusChanged(const QString &status);

    private Q_SLOTS:
        void wiimoteConnected();
        void wiimoteDisconnected();

        void buttonAChanged(bool);
        void buttonBChanged(bool);
        void buttonMinusChanged(bool);
        void buttonHomeChanged(bool);
        void buttonPlusChanged(bool);
        void buttonUpChanged(bool);
        void buttonDownChanged(bool);
        void buttonLeftChanged(bool);
        void buttonRightChanged(bool);
        void buttonOneChanged(bool);
        void buttonTwoChanged(bool);

        void ledOneStateChanged(int state);
        void ledTwoStateChanged(int state);
        void ledThreeStateChanged(int state);
        void ledFourStateChanged(int state);

    private:
        QLabel* m_label;
        KPushButton* m_connectButton;
        Wiimote* m_wiimote;

        Ui::WiimoteInterface ui;

};

#endif // WIIMOTEWIDGET_H

