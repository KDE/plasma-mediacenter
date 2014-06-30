/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "pmccorona.h"
#include <QDebug>

PmcCorona::PmcCorona(QObject *parent)
    : Plasma::Corona(parent)
{
}

QRect PmcCorona::screenGeometry(int id) const
{
    Q_UNUSED(id);
    if(m_view) {
        return m_view->geometry();
    } else {
        return QRect();
    }
}

void PmcCorona::load()
{
    loadLayout("plasma-org.kde.plasma.mediacenter-appletsrc");

    if(containments().isEmpty()) {
        qDebug() << "Tata";
        loadDefaultLayout();
        saveLayout("plasma-org.kde.plasma.mediacenter-appletsrc");
    }
}

void PmcCorona::setView(PlasmaQuick::View *view)
{
    m_view = view;
}


void PmcCorona::loadDefaultLayout()
{
    createContainment("org.kde.desktopcontainment"); 
}

#include "pmccorona.moc"
