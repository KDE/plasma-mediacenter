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

#include <cwiid.h>

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

    bool buttonAPressed;
    bool buttonBPressed;
    bool buttonMinusPressed;
    bool buttonHomePressed;
    bool buttonPlusPressed;
    bool buttonUpPressed;
    bool buttonDownPressed;
    bool buttonLeftPressed;
    bool buttonRightPressed;
    bool buttonOnePressed;
    bool buttonTwoPressed;
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

        void accelerometerXChanged(int);
        void accelerometerYChanged(int);
        void accelerometerZChanged(int);
        void accelerometerChanged(int, int, int);

        void infraredChanged();

    protected:
        void run();

    private Q_SLOTS:
        void updateBattery(struct cwiid_state *state);
        void updateAccelerometers(struct cwiid_acc_mesg acc);
        void updateInfrared(struct cwiid_ir_mesg ir_mesg);

    private:
        void updateButtons(uint16_t buttons);
        QString dumpState(struct cwiid_state *state);
        void setLeds();

        cwiid_wiimote_t *m_wiimote;  /* wiimote handle */
        struct cwiid_state m_wiimoteState;  /* wiimote state */
        bdaddr_t m_bdaddr;
        unsigned char m_ledState;

        struct WiimoteState* m_state;
};

#endif // WIIMOTE_H

