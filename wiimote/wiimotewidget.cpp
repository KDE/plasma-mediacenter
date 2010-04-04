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

// Qt
#include <QBoxLayout>
#include <QLabel>
#include <QTimer>

// KDE
#include <KDebug>
#include <KDialog>
#include <KMainWindow>
#include <KPushButton>

#include "wiimote.h"
#include "wiimotewidget.h"

WiimoteWidget::WiimoteWidget( KMainWindow *parent )
    : QWidget(parent),
    m_infraredDialog(0),
    m_infraredLabel(0),
    m_infraredImage(0)
{
    ui.setupUi(this);
    ui.buttonDisconnect->setEnabled(false);

    m_wiimote = new Wiimote(this);
    m_infraredImage = new InfraredImage(QSize(300, 240));

    // Hook up UI to Wiimote object
    connect(ui.buttonConnect, SIGNAL(clicked()), m_wiimote, SLOT(connectWiimote()));
    connect(ui.buttonDisconnect, SIGNAL(clicked()), m_wiimote, SLOT(disconnectWiimote()));
    connect(ui.buttonRumble, SIGNAL(toggled(bool)), m_wiimote, SLOT(rumble(bool)));
    connect(ui.led1, SIGNAL(stateChanged(int)), SLOT(ledOneStateChanged(int)));
    connect(ui.led2, SIGNAL(stateChanged(int)), SLOT(ledTwoStateChanged(int)));
    connect(ui.led3, SIGNAL(stateChanged(int)), SLOT(ledThreeStateChanged(int)));
    connect(ui.led4, SIGNAL(stateChanged(int)), SLOT(ledFourStateChanged(int)));
    connect(ui.irButton, SIGNAL(clicked()), SLOT(openInfraredDialog()));

    // Hook up Wiimote object to UI
    connect(m_wiimote, SIGNAL(statusChanged(const QString&)),
                this, SLOT(statusChanged(const QString&)));
    connect(m_wiimote, SIGNAL(connected()), this, SLOT(wiimoteConnected()));
    connect(m_wiimote, SIGNAL(disconnected()), this, SLOT(wiimoteDisconnected()));
    connect(m_wiimote, SIGNAL(batteryChanged(int)), ui.battery, SLOT(setValue(int)));

    // The following has to be a blocking connection since we're accessing
    // data from the wiimote's thread in its slots
    connect(m_wiimote, SIGNAL(infraredChanged(QTime)),
            this, SLOT(infraredChanged(QTime)), Qt::BlockingQueuedConnection);

    // Buttons
    connect(m_wiimote, SIGNAL(buttonA(bool)),     this, SLOT(buttonAChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonB(bool)),     this, SLOT(buttonBChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonMinus(bool)), this, SLOT(buttonMinusChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonHome(bool)),  this, SLOT(buttonHomeChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonPlus(bool)),  this, SLOT(buttonPlusChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonUp(bool)),    this, SLOT(buttonUpChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonDown(bool)),  this, SLOT(buttonDownChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonLeft(bool)),  this, SLOT(buttonLeftChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonRight(bool)), this, SLOT(buttonRightChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonOne(bool)),   this, SLOT(buttonOneChanged(bool)));
    connect(m_wiimote, SIGNAL(buttonTwo(bool)),   this, SLOT(buttonTwoChanged(bool)));
    //connect(m_wiimote, SIGNAL(buttonAReleased()), this, SLOT(buttonAChanged()));
    connect(m_wiimote, SIGNAL(nunchukButtonC(bool)), this, SLOT(nunchukButtonCChanged(bool)));
    connect(m_wiimote, SIGNAL(nunchukButtonZ(bool)), this, SLOT(nunchukButtonZChanged(bool)));

    connect(m_wiimote, SIGNAL(accelerometerXChanged(int)), ui.accelX, SLOT(setValue(int)));
    connect(m_wiimote, SIGNAL(accelerometerYChanged(int)), ui.accelY, SLOT(setValue(int)));
    connect(m_wiimote, SIGNAL(accelerometerZChanged(int)), ui.accelZ, SLOT(setValue(int)));

    m_wiimote->start();
}

WiimoteWidget::~WiimoteWidget()
{
    m_wiimote->exit();
}

void WiimoteWidget::buttonAChanged(bool pressed)
{
    ui.buttonA->setDown(pressed);
}

void WiimoteWidget::buttonBChanged(bool pressed)
{
    ui.buttonB->setDown(pressed);
}

void WiimoteWidget::buttonMinusChanged(bool pressed)
{
    ui.buttonMinus->setDown(pressed);
}

void WiimoteWidget::buttonHomeChanged(bool pressed)
{
    ui.buttonHome->setDown(pressed);
}

void WiimoteWidget::buttonPlusChanged(bool pressed)
{
    ui.buttonPlus->setDown(pressed);
}

void WiimoteWidget::buttonUpChanged(bool pressed)
{
    ui.buttonUp->setDown(pressed);
}

void WiimoteWidget::buttonDownChanged(bool pressed)
{
    ui.buttonDown->setDown(pressed);
}

void WiimoteWidget::buttonLeftChanged(bool pressed)
{
    ui.buttonLeft->setDown(pressed);
}

void WiimoteWidget::buttonRightChanged(bool pressed)
{
    ui.buttonRight->setDown(pressed);
}

void WiimoteWidget::buttonOneChanged(bool pressed)
{
    ui.buttonOne->setDown(pressed);
}

void WiimoteWidget::buttonTwoChanged(bool pressed)
{
    ui.buttonTwo->setDown(pressed);
}

void WiimoteWidget::nunchukButtonCChanged(bool pressed)
{
    ui.nunchukButtonC->setDown(pressed);
}

void WiimoteWidget::nunchukButtonZChanged(bool pressed)
{
    ui.nunchukButtonZ->setDown(pressed);
}

void WiimoteWidget::statusChanged(const QString &status)
{
    ui.status->setText(status);
    //kDebug() << "Changed status" << status;
}

void WiimoteWidget::wiimoteConnected()
{
    ui.buttonConnect->setEnabled(false);
    ui.buttonDisconnect->setEnabled(true);
}

void WiimoteWidget::wiimoteDisconnected()
{
    ui.buttonConnect->setEnabled(true);
    ui.buttonDisconnect->setEnabled(false);
    kDebug() << "wiimote disconnected.";
}

void WiimoteWidget::openInfraredDialog()
{
    kDebug() << "opening dialog";
    /*
dialog->setCaption( "My title" );
   dialog->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );

   FooWidget *widget = new FooWidget( dialog );
   dialog->setMainWidget( widget );
   connect( dialog, SIGNAL( applyClicked() ), widget, SLOT( save() ) );
   connect( dialog, SIGNAL( okClicked() ), widget, SLOT( save() ) );
   connect( widget, SIGNAL( changed( bool ) ), dialog, SLOT( enableButtonApply( bool ) ) );

   dialog->enableButtonApply( false );
   dialog->show();
   */
    if (m_infraredDialog) {
        m_infraredDialog->show();
        return;
    }
    m_infraredDialog = new KDialog(this);
    m_infraredDialog->setCaption("Infrared Sensors");
    m_infraredDialog->setButtons(KDialog::Close);
    m_infraredLabel = new QLabel(m_infraredDialog);
    m_infraredDialog->setMainWidget(m_infraredLabel);
    m_infraredDialog->show();
}

void WiimoteWidget::infraredChanged(QTime t)
{
    int diff = QTime::currentTime().msec() - t.msec();
    if (diff > 5) { // Skip events that are older than 5 msec to throttle the framerate a bit
        //kDebug() << "skipping, Age:" << diff;
        return;
    }
    if (m_infraredDialog) {
        QList<QPoint> leds = m_wiimote->state()->infrared;
        m_infraredImage->state = m_wiimote->state();
        m_infraredImage->resize(m_infraredLabel->geometry().size());
        QStringList _out;
        foreach (QPoint p, leds) {
            _out << QString("x: %1 y: %2 ").arg(p.x()).arg(p.y());
        }
        //m_infraredLabel->setText(_out.join("<br />"));
        m_infraredImage->setLeds(leds);
        m_infraredLabel->setPixmap(m_infraredImage->pixmap());
    }
}

void WiimoteWidget::ledOneStateChanged(int state)
{
    if (state == Qt::Checked) {
        m_wiimote->ledOneOn();
    } else {
       m_wiimote->ledOneOff();
    }
}

void WiimoteWidget::ledTwoStateChanged(int state)
{
    if (state == Qt::Checked) {
        m_wiimote->ledTwoOn();
    } else {
       m_wiimote->ledTwoOff();
    }
}

void WiimoteWidget::ledThreeStateChanged(int state)
{
    if (state == Qt::Checked) {
        m_wiimote->ledThreeOn();
    } else {
       m_wiimote->ledThreeOff();
    }
}

void WiimoteWidget::ledFourStateChanged(int state)
{
    if (state == Qt::Checked) {
        m_wiimote->ledFourOn();
    } else {
       m_wiimote->ledFourOff();
    }
}

#include "wiimotewidget.moc"
