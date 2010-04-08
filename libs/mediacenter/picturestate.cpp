/***************************************************************************
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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

#include "picturestate.h"

#include <mediacenter/browser.h>
#include <mediacenter/playbackcontrol.h>
#include <mediacenter/playlist.h>
#include <mediacenter/player.h>

using namespace MediaCenter;

PictureState::PictureState (QState *parent)
    : MediaCenterState(parent),
    m_pictureStateLabel(new Plasma::IconWidget())
{
}

PictureState::~PictureState()
{
}

void PictureState::onExit(QEvent* event)
{
}

void PictureState::onEntry(QEvent* event)
{
    MediaCenter::State newState = MediaCenter::PictureMode;
    emit state(newState);
    kWarning() << "In Picture State";
}

QList<QGraphicsWidget*> PictureState::subComponents()
{
    QList<QGraphicsWidget*> list;

    m_pictureStateLabel->setText("Picture State");

    return list << m_pictureStateLabel;
}

void PictureState::configureUIComponents(QList<Plasma:: Applet*> list)
{
      foreach (Plasma::Applet *applet, list) {
        MediaCenter::Browser *browser = qobject_cast<MediaCenter::Browser*>(applet);
        if (browser) {
        }
        MediaCenter::Player *player = qobject_cast<MediaCenter::Player*>(applet);
        if (player) {
            player->setVisible(false);
        }
        MediaCenter::PlaybackControl *control = qobject_cast<MediaCenter::PlaybackControl*>(applet);
        if (control) {
        }
        MediaCenter::Playlist *playlist = qobject_cast<MediaCenter::Playlist*>(applet);
        if (playlist) {
            playlist->setVisible(false);
        }
    }
}

void PictureState::connectSubComponents(QList<Plasma:: Applet*> list)
{
}

