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

#ifndef MEDIACENTERSTATE_H
#define MEDIACENTERSTATE_H

#include "mediacenter.h"
#include "mediacenter_export.h"

#include <QState>
#include <QGraphicsWidget>

#include <Plasma/IconWidget>
#include <Plasma/Applet>

namespace MediaCenter {

enum State {
    PictureMode,
    MusicMode,
    VideoMode,
    Home
};

enum MainSubComponent {
    JumpToVideoState,
    JumpToMusicState,
    JumpToPictureState,
    JumpToHome,
    ToggleControlBarAutohide,
    ToggleInfoBarAutohide
};
class MediaLayout;

class MEDIACENTER_EXPORT MediaCenterState : public QState
{
    Q_OBJECT
public:
    MediaCenterState(QState *parent = 0);
    virtual ~MediaCenterState();

    //This function hands out all main subcomponents to the UIcomponents
    QList<QGraphicsWidget*> mainSubComponents();
    //This handles connections of MainSubComponents
    void connectMainSubComponents(QList<Plasma:: Applet*> list);

    /**
     *This function should be reimplemented by every state to connect subcomponents
     *with slots in the UIComponents
     */
    virtual void connectSubComponents(QList<Plasma:: Applet*> list);

    /**
     *This function handles connections of the medialayout and should be reimplemented
     *to connect the layout slots to subcomponents of that state
     */
    virtual void connectMediaLayout(MediaLayout* layout);

    /**
     *This function will exectute functions handled by the media layout for each state
     */
    virtual void configureMediaLayout(MediaLayout* layout);

    /**
     *This function should be reimplemented by every state to handle UICComponents' configuration
     */
    virtual void configureUIComponents(QList<Plasma:: Applet*> list);

    /**
     *Every state needs a list of subComponents for the UIApplets. The UIApplets should recognize
     *which subComponents arrive and do the layouting accordingly
     */
    virtual QList<QGraphicsWidget*> subComponents();

signals:
     void state(MediaCenter::State);

protected:
    virtual void onExit(QEvent* event);
    virtual void onEntry(QEvent* event);

    Plasma::IconWidget *m_jumpToHome;
    Plasma::IconWidget *m_jumpToVideo;
    Plasma::IconWidget *m_jumpToPicture;
    Plasma::IconWidget *m_jumpToMusic;
    Plasma::IconWidget *m_toggleControlBarAutohide;
};

} //namespace MediaCenter
#endif // MEDIACENTERSTATE_H
