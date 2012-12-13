/*
    Copyright (C) 2012 Shantanu Tushar <shantanu@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "fullscreenbackend.h"
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QtCore/QMetaObject>

MEDIACENTER_EXPORT_BROWSINGBACKEND(FullscreenBackend)

FullscreenBackend::FullscreenBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args)
{

}

bool FullscreenBackend::initImpl()
{
    QMainWindow *mainWindow = 0;
    QWidget *widget = QApplication::activeWindow();
    while (1) {
        mainWindow = qobject_cast<QMainWindow*>(widget);
        if (mainWindow)
            break;
        widget = widget->parentWidget();
        if (!widget)
            return false;
    }

    QMetaObject::invokeMethod(mainWindow, "toggleFullScreen");
    return false;
}
