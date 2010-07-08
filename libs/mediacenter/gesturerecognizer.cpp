/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "gesturerecognizer.h"

#include <QEvent>
#include <QKeyEvent>

#include <mediacenter/playbackcontrol.h>
#include <mediacenter/browser.h>
#include <mediacenter/playbackgesture.h>
#include <mediacenter/browsergesture.h>

#include <KDebug>

namespace MediaCenter
{

GestureRecognizer::GestureRecognizer() : QGestureRecognizer()
{
}

GestureRecognizer::~GestureRecognizer()
{}

QGesture* GestureRecognizer::create(QObject *target)
{
    kDebug() << "creating gesture for target" << target;
    PlaybackControl *controller = qobject_cast<PlaybackControl*>(target);
    if (controller) {
        return (new MediaCenter::PlaybackGesture);
    }

    Browser *browser = qobject_cast<Browser*>(target);
    if (browser) {
        return (new MediaCenter::BrowserGesture);
    }

    return QGestureRecognizer::create(target);
}

QGestureRecognizer::Result GestureRecognizer::recognize(QGesture *state, QObject *watched, QEvent *event)
{
    // we are filtering gestures for the controller appletqeven
    MediaCenter::PlaybackGesture *playbackGesture = qobject_cast<MediaCenter::PlaybackGesture*>(state);
    if (playbackGesture) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Plus) {
                playbackGesture->increaseVolume();
                playbackGesture->m_activeAction = PlaybackGesture::VolumeAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Minus) {
                playbackGesture->decreaseVolume();
                playbackGesture->m_activeAction = PlaybackGesture::VolumeAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Left && e->modifiers() & Qt::ControlModifier) {
                playbackGesture->m_activeAction = PlaybackGesture::SkipBackward;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Right && e->modifiers() & Qt::ControlModifier) {
                playbackGesture->m_activeAction = PlaybackGesture::SkipForward;
                return QGestureRecognizer::TriggerGesture;
            }
        } else {
            return QGestureRecognizer::CancelGesture;
        }
    }

    // we are filtering gestures for the browser applet
    MediaCenter::BrowserGesture *browserGesture = qobject_cast<MediaCenter::BrowserGesture*>(state);
    if (browserGesture) {
        MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(watched);
        Q_ASSERT(browser);

        if (event->type() == QEvent::KeyPress) {
            browserGesture->setHotSpot(browser->geometry().center());
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
                e->accept();
                browserGesture->m_activeAction = BrowserGesture::ActivateAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Left) {
                e->accept();
                browserGesture->m_activeAction = BrowserGesture::BrowseLeftAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Right) {
                e->accept();
                browserGesture->m_activeAction = BrowserGesture::BrowseRightAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Down) {
                e->accept();
                browserGesture->m_activeAction = BrowserGesture::BrowseDownAction;
                return QGestureRecognizer::TriggerGesture;
            } else if (e->key() == Qt::Key_Up) {
                e->accept();
                browserGesture->m_activeAction = BrowserGesture::BrowseUpAction;
                return QGestureRecognizer::TriggerGesture;
            }
        }
    } else {
        event->ignore();
        return QGestureRecognizer::CancelGesture;
    }

    return QGestureRecognizer::CancelGesture;
}

void GestureRecognizer::reset(QGesture *state)
{
    MediaCenter::PlaybackGesture *playbackGesture = qobject_cast<MediaCenter::PlaybackGesture*>(state);
    if (playbackGesture) {
        playbackGesture->m_volumeLevel = 0;
        playbackGesture->m_activeAction = PlaybackGesture::NoAction;
    }
}
}
