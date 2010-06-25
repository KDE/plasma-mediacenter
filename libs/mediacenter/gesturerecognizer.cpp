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
#include <mediacenter/playbackgesture.h>

using namespace MediaCenter;
GestureRecognizer::GestureRecognizer()
{
}

QGesture* GestureRecognizer::create(QObject *target)
{
    PlaybackControl *controller = qobject_cast<PlaybackControl*>(target);
    if (controller) {
        return (new MediaCenter::PlaybackGesture);
    }
}

QGestureRecognizer::Result GestureRecognizer::recognize(QGesture *state, QObject *watched, QEvent *event)
{
    // we are filtering gestures for the controller applet
    MediaCenter::PlaybackGesture *playbackGesture = qobject_cast<MediaCenter::PlaybackGesture*>(state);
    if (playbackGesture) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *e = static_cast<QKeyEvent*>(event);
            if (e->key() == Qt::Key_Plus) {
                playbackGesture->increaseVolume();
                return QGestureRecognizer::FinishGesture;
            } else if (e->key() == Qt::Key_Minus) {
                playbackGesture->decreaseVolume();
                return QGestureRecognizer::FinishGesture;
            }
        }
    }
}

void GestureRecognizer::reset(QGesture *state)
{
    MediaCenter::PlaybackGesture *playbackGesture = qobject_cast<MediaCenter::PlaybackGesture*>(state);
    if (playbackGesture) {
        playbackGesture->m_volumeLevel = 0;
        playbackGesture->m_activeAction = PlaybackGesture::NoAction;
    }
}
