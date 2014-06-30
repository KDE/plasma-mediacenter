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
#include "pmcview.h"
#include <QDebug>

#include <Plasma/Package>
#include <Plasma/PluginLoader>

PmcCorona::PmcCorona(QObject *parent)
    : Plasma::Corona(parent),
      m_view(0)
{
    Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Shell");
    package.setPath("org.kde.plasma.mediacenter");
    setPackage(package);
    //QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection);
    load();
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

    bool found = false;
    for (auto c : containments()) {
        if (c->containmentType() == Plasma::Types::DesktopContainment) {
            found = true;
            break;
        }
    }

    if (!found) {
        qDebug() << "Loading default layout";
        loadDefaultLayout();
        saveLayout("plasma-org.kde.plasma.mediacenter-appletsrc");
    }

    for (auto c : containments()) {
        qDebug() << "here we are!";
        if (c->containmentType() == Plasma::Types::DesktopContainment) {
            m_view = new PmcView(this);
            setView(m_view);
            m_view->setContainment(c);
            m_view->show();
            break;
        }
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
