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
#include "medialayout.h"

#include <Plasma/Containment>
#include <Plasma/Applet>

MediaLayout::MediaLayout(Plasma::Containment *parent) : QObject(parent),
m_containment(parent),
m_browser(0),
m_control(0)
{
    m_containment->installEventFilter(this);
}

MediaLayout::~MediaLayout()
{
}

void MediaLayout::setBrowser(Plasma::Applet *browser)
{
    m_browser = browser;
    m_needLayouting << m_browser;
}

void MediaLayout::setPlaybackControl(Plasma::Applet *control)
{
    m_control = control;
    m_needLayouting << m_control;
}

void MediaLayout::invalidate()
{
    foreach (Plasma::Applet *applet, m_needLayouting) {
        if (applet == m_browser) {
            layoutBrowser();
        } else if (applet == m_control) {
            layoutControl();
        }
    }
}

void MediaLayout::doCompleteLayout()
{
    if (m_browser) {
        layoutBrowser();
    }
    if (m_control) {
        layoutControl();
    }
}

void MediaLayout::layoutBrowser()
{
    m_browser->resize(m_containment->size().width() / 3.0, m_containment->size().height() * 2 / 3.0);
    m_browser->setPos(0, (m_containment->size().height() - m_browser->size().height()) / 2);
}

void MediaLayout::layoutControl()
{}

bool MediaLayout::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_containment && e->type() == QEvent::GraphicsSceneResize) {
        doCompleteLayout();
    }

    return false;
}
