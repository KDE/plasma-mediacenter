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
#include "mediacontainment.h"
#include <mediacenter/browser.h>

// Qt
#include <QAction>

// KDE
#include <KDebug>
#include <KNotification>
#include <KLocale>

K_EXPORT_PLASMA_APPLET(mediacontainment, MediaContainment)

MediaContainment::MediaContainment(QObject *parent, const QVariantList &args) : Plasma::Containment(parent, args),
m_browser(0)
{
    setHasConfigurationInterface(true);
    setAcceptHoverEvents(true);
}

MediaContainment::~MediaContainment()
{}

QList<QAction*> MediaContainment::contextualActions()
{
    QList<QAction*> actions;

    actions << action("add widgets");

    return actions;
}

void MediaContainment::constraintsEvent(Plasma::Constraints constraints)
{
    if (constraints & Plasma::StartupCompletedConstraint) {
        connect (this, SIGNAL(appletAdded(Plasma::Applet*, const QPointF &)), SLOT(slotAppletAdded(Plasma::Applet*, const QPointF &)));
        connect (this, SIGNAL(appletRemoved(Plasma::Applet*)), SLOT(slotAppletRemoved(Plasma::Applet*)));
    }
}

void MediaContainment::slotAppletAdded(Plasma::Applet *applet, const QPointF &pos)
{
    Q_UNUSED(pos)

    // browser check
    MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
    if (browser && m_browser) {
        KNotification::event(KNotification::Error, i18n("A browser for the mediacenter already is loaded "
                                                        "in this containment. Remove that one before loading a new one please."));
        applet->destroy();
    }
    m_browser = browser;

}

void MediaContainment::slotAppletRemoved(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        m_browser = 0;
    }
}
