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

#ifndef WIIMOTE_H
#define WIIMOTE_H

#include <QThread>
#include <QPoint>
#include <iostream>

#include <cwiid.h>

//typedef QList<QPoint> QPointList;



struct WiimoteState {

    bool connected;
    QString deviceName;
    QString deviceClass;
    QString btAddress;
    int battery;

    bool ledOne;
    bool ledTwo;
    bool ledThree;
    bool ledFour;

    int accelX;
    int accelY;
    int accelZ;

    QList<QPoint> infrared;

    int nunchukStickX;
    int nunchukStickY;

    bool buttonAPressed : 1;
    bool buttonBPressed : 1;
    bool buttonMinusPressed : 1;
    bool buttonHomePressed : 1;
    bool buttonPlusPressed : 1;
    bool buttonUpPressed : 1;
    bool buttonDownPressed : 1;
    bool buttonLeftPressed : 1;
    bool buttonRightPressed : 1;
    bool buttonOnePressed : 1;
    bool buttonTwoPressed : 1;
    bool nunchukButtonCPressed : 1;
    bool nunchukButtonZPressed : 1;
};


class Wiimote : public QThread
{
    Q_OBJECT

    public:
        Wiimote( QObject* parent=0 );
        ~Wiimote();

        struct WiimoteState* state();

        bool ledOne();
        bool ledTwo();
        bool ledThree();
        bool ledFour();

        int scrollSpeed();
        void setScrollSpeed(int);

        static void cwiidCallback(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp);
        void wiimoteEvent(cwiid_wiimote_t *wiimote, int mesg_count,
                    union cwiid_mesg mesg[], struct timespec *timestamp);

    public Q_SLOTS:
        void updateStatus();
        void disconnectWiimote();
        bool connectWiimote();

        void ledOneOn();
        void ledOneOff();
        void ledTwoOn();
        void ledTwoOff();
        void ledThreeOn();
        void ledThreeOff();
        void ledFourOn();
        void ledFourOff();

        void rumble(bool);

    Q_SIGNALS:
        void statusChanged(const QString&);
        void connected();
        void disconnected();
        void batteryChanged(int);

        void buttonA(bool);
        void buttonB(bool);
        void buttonMinus(bool);
        void buttonHome(bool);
        void buttonPlus(bool);
        void buttonUp(bool);
        void buttonDown(bool);
        void buttonLeft(bool);
        void buttonRight(bool);
        void buttonOne(bool);
        void buttonTwo(bool);
        void nunchukButtonC(bool);
        void nunchukButtonZ(bool);
        void nunchukStickChanged(int, int);

        void accelerometerXChanged(int);
        void accelerometerYChanged(int);
        void accelerometerZChanged(int);
        void accelerometerChanged(int, int, int);

        void infraredChanged(QTime);

        /* Gestures */
        void dragUpDown(int);
        void flickUpDown(int);

    protected:
        void run();

    private Q_SLOTS:
        void updateBattery(struct cwiid_state *state);
        void updateAccelerometers(struct cwiid_acc_mesg acc);
        void updateInfrared(struct cwiid_ir_mesg ir_mesg);
        void knightRider();

    private:
        void updateButtons(uint16_t buttons);
        void updateNunchukButtons(uint8_t buttons);
        void updateNunchuk(struct cwiid_nunchuk_mesg);
        QString dumpState(struct cwiid_state *state);
        void setLeds();

        cwiid_wiimote_t *m_wiimote;  /* wiimote handle */
        struct cwiid_state m_wiimoteState;  /* wiimote state */
        bdaddr_t m_bdaddr;
        unsigned char m_ledState;

        struct WiimoteState* m_state;

        QTimer* m_knightRiderTimer;
        int m_knightRiderState;
        bool _knUp;

        int m_scrollSpeed;
};

//Q_DECLARE_METATYPE(QPointList)
//qRegisterMetaType<QPointList>("QPointList");

#endif // WIIMOTE_H

