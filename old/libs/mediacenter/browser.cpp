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
#include "browsergesture.h"
#include "abstractbrowsingbackend.h"
#include "widgets/navigationtoolbar.h"

#include <QGestureEvent>
#include <QGraphicsLinearLayout>

#include <KDebug>

using namespace MediaCenter;

class Browser::BrowserPrivate
{
public:
    BrowserPrivate(Browser *q) :
        q(q),
        toolbar(0),
        enableToolbar(false),
        currentBackend(0)
    {}

    Browser *q;
    Qt::GestureType gestureType;
    NavigationToolbar *toolbar;
    bool enableToolbar : 1;
    AbstractBrowsingBackend *currentBackend;
};

Browser::Browser(QObject *parent, const QVariantList &args) : Plasma::Applet(parent, args),
    d(new BrowserPrivate(this))
{
}

Browser::~Browser()
{
    delete d;
}

bool Browser::sceneEvent(QEvent *event)
{
    if (event->type() == QEvent::Gesture) {
        QGestureEvent *gEvent = static_cast<QGestureEvent*>(event);
        MediaCenter::BrowserGesture *bGesture = qobject_cast<MediaCenter::BrowserGesture*>(gEvent->gesture(d->gestureType));

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
    d->gestureType = type;
}

MediaCenter::NavigationToolbar* Browser::toolbar() const
{
    return d->toolbar;
}

void Browser::setEnableToolbar(bool set)
{
    if (set == d->enableToolbar) {
        return;
    }

    d->enableToolbar = set;

    if (d->enableToolbar) {
        if (!d->toolbar) {
            d->toolbar = new NavigationToolbar(this);
            connect(d->toolbar, SIGNAL(browseHistoryBack()), this, SIGNAL(browseHistoryBackRequest()));
            connect(d->toolbar, SIGNAL(browseHistoryNext()), this, SIGNAL(browseHistoryNextRequest()));
            connect(d->toolbar, SIGNAL(browseUp()), this, SIGNAL(browseUpRequest()));
            connect(d->toolbar, SIGNAL(browsePath(QString)), this, SIGNAL(browsePathRequest(QString)));

            QGraphicsLinearLayout *linearLayout = dynamic_cast<QGraphicsLinearLayout*>(layout());
            QGraphicsLayout *internalLayout;
            if (!linearLayout) { // widget's layout is not a linear one: we create one
                internalLayout = layout();
                setLayout(0);
                linearLayout = new QGraphicsLinearLayout(Qt::Vertical);
                linearLayout->addItem(d->toolbar);
                linearLayout->addItem(internalLayout);
            } else {
                if (linearLayout->orientation() != Qt::Vertical) { // widget's layout is linear but not vertical: we create one
                    internalLayout = linearLayout;
                    linearLayout = new QGraphicsLinearLayout(Qt::Vertical);
                    linearLayout->addItem(d->toolbar);
                    linearLayout->addItem(internalLayout);
                } else { // widget's layout is already linear and vertical: we simply add the toolbar on top
                    linearLayout->insertItem(0, d->toolbar);
                }
            }
            setLayout(linearLayout);
        }

        d->toolbar->show();
    } else {
        if (d->toolbar) {
            d->toolbar->hide();
        }
    }
}

bool Browser::enableToolbar() const
{
    return d->enableToolbar;
}

AbstractBrowsingBackend* Browser::currentBrowsingBackend() const
{
    return d->currentBackend;
}

void Browser::setCurrentBrowsingBackend(AbstractBrowsingBackend *backend)
{
    if (d->currentBackend == backend) {
        return;
    }
    d->currentBackend = backend;
    loadBrowsingBackend(d->currentBackend);
}

