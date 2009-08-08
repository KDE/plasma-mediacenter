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
#include "browsingwidget.h"

// Qt
#include <QGraphicsLinearLayout>
// KDE
#include <KIcon>
// Plasma
#include <Plasma/PushButton>

BrowsingWidget::BrowsingWidget(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_backwardButton(0),
m_forwardButton(0),
m_upLevelButton(0)
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    setLayout(layout);

    setNavigationControls(AllControls);
    setShowNavigationComboBox(false);
}

BrowsingWidget::~BrowsingWidget()
{}

void BrowsingWidget::setNavigationControls(NavigationControls controls)
{
    m_controls = controls;
    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());

    if (m_backwardButton) {
        layout->removeItem(m_backwardButton);
    }
    if (m_forwardButton) {
        layout->removeItem(m_forwardButton);
    }
    if (m_upLevelButton) {
        layout->removeItem(m_upLevelButton);
    }

    if (m_controls == NoControls) {
        if (m_backwardButton) {
            layout->removeItem(m_backwardButton);
            delete m_backwardButton;
            m_backwardButton = 0;
        }
        if (m_forwardButton) {
            layout->removeItem(m_forwardButton);
            delete m_forwardButton;
            m_forwardButton = 0;
        }
        if (m_upLevelButton) {
            layout->removeItem(m_upLevelButton);
            delete m_upLevelButton;
            m_upLevelButton = 0;
        }
        return;
    }

    if (m_controls & UpLevelControl) {
        if (!m_upLevelButton) {
            m_upLevelButton = new Plasma::PushButton(this);
            m_upLevelButton->setIcon(KIcon("go-up"));
        }
        layout->insertItem(0, m_upLevelButton);
    }

    if (m_controls & ForwardControl) {
        if (!m_forwardButton) {
            m_forwardButton = new Plasma::PushButton(this);
            m_forwardButton->setIcon(KIcon("go-next"));
        }
        layout->insertItem(0, m_forwardButton);
    }

    if (m_controls & BackwardControl) {
        if (!m_backwardButton) {
            m_backwardButton = new Plasma::PushButton(this);
            m_backwardButton->setIcon(KIcon("go-previous"));
        }
        layout->insertItem(0, m_backwardButton);
    }

    setLayout(layout);
}

NavigationControls BrowsingWidget::navigationControls()
{
    return m_controls;
}
