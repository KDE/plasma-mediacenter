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
#include "navigationtoolbar.h"

// Qt
#include <QGraphicsLinearLayout>
// KDE
#include <KIcon>
#include <KComboBox>
#include <KDebug>
// Plasma
#include <Plasma/ToolButton>
#include <Plasma/ComboBox>
#include <Plasma/TabBar>

using namespace MediaCenter;

class NavigationToolbar::NavigationToolbarPrivate
{
public:
    NavigationToolbarPrivate(NavigationToolbar *q) :
        q(q),
        controls(AllControls),
        navigationCombo(false),
        backwardButton(0),
        forwardButton(0),
        upLevelButton(0),
        combo(0)
    {}

    NavigationToolbar *q;
    NavigationControls controls;
    bool navigationCombo;

    Plasma::ToolButton *backwardButton;
    Plasma::ToolButton *forwardButton;
    Plasma::ToolButton *upLevelButton;

    Plasma::ComboBox *combo;
};

NavigationToolbar::NavigationToolbar(QGraphicsItem *parent) : QGraphicsWidget(parent),
d(new NavigationToolbarPrivate(this))
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    setLayout(layout);

    setNavigationControls(BackwardControl);
    setShowNavigationComboBox(false);
}

NavigationToolbar::~NavigationToolbar()
{
    delete d;
}

void NavigationToolbar::setNavigationControls(NavigationControls controls)
{
    d->controls = controls;
    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());

    if (d->backwardButton) {
        layout->removeItem(d->backwardButton);
    }
    if (d->forwardButton) {
        layout->removeItem(d->forwardButton);
    }
    if (d->upLevelButton) {
        layout->removeItem(d->upLevelButton);
    }

    if (d->controls == NoControls) {
        kDebug() << "no controls!!";
        if (d->backwardButton) {
            layout->removeItem(d->backwardButton);
            delete d->backwardButton;
            d->backwardButton = 0;
        }
        if (d->forwardButton) {
            layout->removeItem(d->forwardButton);
            delete d->forwardButton;
            d->forwardButton = 0;
        }
        if (d->upLevelButton) {
            layout->removeItem(d->upLevelButton);
            delete d->upLevelButton;
            d->upLevelButton = 0;
        }
        return;
    }

    if (d->controls & UpLevelControl) {
        if (!d->upLevelButton) {
            d->upLevelButton = new Plasma::ToolButton(this);
            d->upLevelButton->setIcon(KIcon("go-up"));
            connect (d->upLevelButton, SIGNAL(clicked()), this, SIGNAL(browseUp()));
        }
        layout->insertItem(0, d->upLevelButton);
    }

    if (d->controls & ForwardControl) {
        if (!d->forwardButton) {
            d->forwardButton = new Plasma::ToolButton(this);
            d->forwardButton->setIcon(KIcon("go-next"));
            connect (d->forwardButton, SIGNAL(clicked()), this, SIGNAL(browseHistoryNext()));
        }
        layout->insertItem(0, d->forwardButton);
    }

    if (d->controls & BackwardControl) {
        if (!d->backwardButton) {
            d->backwardButton = new Plasma::ToolButton(this);
            d->backwardButton->setIcon(KIcon("go-previous"));
            connect (d->backwardButton, SIGNAL(clicked()), this, SIGNAL(browseHistoryBack()));
        }
        layout->insertItem(0, d->backwardButton);
    }

    setLayout(layout);
}

NavigationToolbar::NavigationControls NavigationToolbar::navigationControls()
{
    return d->controls;
}

void NavigationToolbar::setShowNavigationComboBox(bool set)
{
    if (d->navigationCombo == set) {
        return;
    }

    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
    d->navigationCombo = set;
    if (d->navigationCombo) {
        if (!d->combo) {
            d->combo = new Plasma::ComboBox(this);
            d->combo->nativeWidget()->setEditable(true);
            connect (d->combo->nativeWidget(), SIGNAL(returnPressed(const QString&)), this, SIGNAL(browsePath(const QString&)));
        }
        layout->addItem(d->combo);
    } else {
        layout->removeItem(d->combo);
        delete d->combo;
        d->combo = 0;
    }
    setLayout(layout);
}
