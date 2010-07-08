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
#include "browser.h"
#include <mediacenter/browsergesture.h>

#include <QGestureEvent>

#include <KDebug>

using namespace MediaCenter;

Browser::Browser(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args)
{
}

Browser::~Browser()
{}

bool Browser::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture) {
        QGestureEvent *gEvent = static_cast<QGestureEvent*>(event);
        MediaCenter::BrowserGesture *bGesture = qobject_cast<MediaCenter::BrowserGesture*>(gEvent->gesture(m_gestureType));

        Q_ASSERT(bGesture);

        gestureEvent(bGesture);
        return true;
    }

    return Plasma::Applet::sceneEvent(event);
}

void Browser::gestureEvent(MediaCenter::BrowserGesture *gesture)
{
    Q_UNUSED(gesture)
}

void Browser::setGestureType(Qt::GestureType type)
{
    m_gestureType = type;
}
