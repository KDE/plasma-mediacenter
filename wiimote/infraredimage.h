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

#ifndef INFRAREDIMAGE_H
#define INFRAREDIMAGE_H

#include <wiimote.h>

#include <QImage>

class InfraredImage : public QObject
{
    Q_OBJECT

    public:
        InfraredImage(const QSize &size = QSize(800, 600), QObject* parent = 0);
        ~InfraredImage();

        QPixmap pixmap();
        void resize(const QSize &s);

        struct WiimoteState *state;

    public Q_SLOTS:
        //void statusChanged(const QString &status);
        //void ledsChanged();
        void setLeds(QList<QPoint>);

    private:
        QList<QPoint> leds;
        QPixmap m_image;

};

#endif // WIIMOTEWIDGET_H

