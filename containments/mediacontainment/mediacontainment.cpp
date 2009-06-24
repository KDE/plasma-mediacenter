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
#include "medianotificationwidget.h"
#include "medialayout.h"
#include "mediahandler.h"
#include <browser.h>

// Qt
#include <QAction>

// KDE
#include <KDebug>
#include <KNotification>
#include <KLocale>

static const int BROWSER_WIDTH = 300;
static const int BROWSER_HEIGHT = 100;

static const int HANDLER_HEIGHT = 140;
static const int HANDLER_WIDTH = 10;

K_EXPORT_PLASMA_APPLET(mediacontainment, MediaContainment)

MediaContainment::MediaContainment(QObject *parent, const QVariantList &args) : Plasma::Containment(parent, args),
m_browser(0),
m_layout(new MediaLayout(this))
{
    setContainmentType(Plasma::Containment::CustomContainment);
    setHasConfigurationInterface(true);
    setAcceptHoverEvents(true);
}

MediaContainment::~MediaContainment()
{}

QList<QAction*> MediaContainment::contextualActions()
{
    QList<QAction*> actions;

    actions << action("add widgets");
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    actions << separator;
    actions << action("configure");

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
    if (browser) {
        if (m_browser) {
            KNotification::event(KNotification::Error, i18n("A browser for the Media Center is already loaded. "
                                                            "Remove that one before loading a new one please."));
            kDebug() << "destroying applet";
            applet->deleteLater();
        } else {
            m_browser = browser;
            m_layout->setBrowser(m_browser);
            MediaHandler *handler = new MediaHandler(m_browser, MediaHandler::Right);
            handler->resize(HANDLER_WIDTH, HANDLER_HEIGHT);
            m_layout->invalidate();
        }
    }

}

void MediaContainment::slotAppletRemoved(Plasma::Applet *applet)
{
    if (applet == m_browser) {
        m_browser = 0;
    }
}
