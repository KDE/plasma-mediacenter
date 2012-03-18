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
//#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QTimer>

// KDE
#include <KDebug>
//#include <KMainWindow>
#include <KPushButton>
#include "infraredimage.h"

const QSize irResolution = QSize(600, 600);

InfraredImage::InfraredImage(const QSize &size, QObject* parent)
    : QObject(parent)
{
    state = new WiimoteState;
    m_image = QPixmap(size);
    QList<QPoint> leds;
    leds << QPoint(40, 400) << QPoint(300, 600);
    setLeds(leds);
    emit changed();
}

void InfraredImage::setLeds(QList<QPoint> leds)
{
    // LED sensor has resolution of 1015x760
    m_image.fill(Qt::transparent);
    //m_image.fill(Qt::white);
    QPen pen;
    if (!state->buttonAPressed) {
        pen = QPen(Qt::red);
    } else {
        pen = QPen(Qt::green);
    }
    pen.setWidth(3);
    //kDebug() << "led" << leds;
    QPainter painter(&m_image);
    painter.setPen(pen);
    //painter.setFont(QFont("Arial", 30));
    //painter.drawRect(QRect(0,0, 300, 240), Qt::AlignCenter, "Qt");
    QList<QPointF> l;
    foreach(const QPoint &led, leds) {
        int w = m_image.width();
        qreal xFactor = (qreal)(w) / (qreal)(irResolution.width());
        //if (w > irResolution.x()) {
        int x = w - qAbs(led.x() * xFactor); //
        //kDebug() << "W / X / XFactor:" << w << led.x() << xFactor << " = "<< (led.x() * xFactor) << " -> " << x;

        int h = m_image.height();
        qreal yFactor = (qreal)(h) / (qreal)(irResolution.height());
        //if (w > irResolution.x()) {
        int y = led.y() * yFactor;

        
        //kDebug() << led << irResolution.width() << (qreal)(led.x()) / (qreal)(irResolution.width()) << "X" << x << "W" << w << "" << "xFactor" << xFactor;
        //} else {

        //}
        l << QPointF(x,y);
        //kDebug() << QRect(QPoint(x, y), QSize(8, 8));
        painter.drawEllipse(QRect(QPoint(x, y), QSize(8, 8)));
    }
    //pen.setOpacity(.5);
    //painter.setPen(pen);
    painter.drawEllipse(QRect(QPoint(300, 300), QSize(12, 12)));
    //kDebug() << "points:" << l;
    painter.drawText(QPointF(0,0), "This works");
    painter.end();
    //emit changed();
}

QPixmap InfraredImage::pixmap()
{
    return m_image;
}

void InfraredImage::resize(const QSize &s)
{
    if (m_image.rect().size() != s) {
        kDebug() << "Size changed, scaling";
        m_image = m_image.scaled(s);
    }
}

InfraredImage::~InfraredImage()
{
    //delete m_image;
    delete state;
}

#include "infraredimage.moc"
