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

#include <QtCore/QState>
#include <QtGui/QGraphicsWidget>

#include <Plasma/IconWidget>
#include <Plasma/Applet>

namespace MediaCenter {

/**
 * Each new Mode has to be added to this enum list. A mode is identical to a
 * state in the state machine.
 */
enum Mode {
    PictureMode,
    MusicMode,
    VideoMode,
    HomeMode
};

class MediaLayout;
class Player;
class PlaybackControl;
class Playlist;
class Browser;
class InfoDisplay;
class HomeApplet;

class MEDIACENTER_EXPORT MediaCenterState : public QState
{
    Q_OBJECT
public:
    MediaCenterState(QState *parent = 0);
    virtual ~MediaCenterState();

    /**
     * Use this function to option pointers to all subcomponents that are shared by the different
     * states. While defining the widgets this function also adds them to the layouts of the
     * different applets. The returned list can be used to keep track of the widgets that
     * currently are in use.
     */
    QList<QGraphicsWidget*> mainSubComponents() const;

    void init(MediaLayout* &layout,QList<Plasma:: Applet*> &list);

    /**
     * This handles all connections for this state, except conflicting connections
     * which need to be handled in the entry and exit functions. Reimplement this in
     * every state.
     */
    virtual void initConnections();

    /**
     * This function will configure all components and the layout. For example to turn off or on
     * the autohide of applets or show/hide applets, etc.
     */
    virtual void configure();

    /**
     * This function hands out a list of pointers to wigets that are specific to this state.
     * It also hands these points to the addToLayout functions of the applets by joining
     * to the pointers the information of in which zone the widget should go.
     */
    virtual QList<QGraphicsWidget*> subComponents() const;

Q_SIGNALS:
    /**
     * This signal should be emitted by each state on entry and causes the containment to actually
     * call the important fucntions like addToLayout, or configure of each state when switched".
     * So first: the below signals cause a state change
     *    second: the containemnt uses this signal emitted onEntry of each state
     */
    void state(const MediaCenter::Mode &mode);

    /**
     * These signals are the actual state switch initiators. They are connected to the state machine
     * in the containment
     */
    void pictureModeRequired();
    void videoModeRequired();
    void musicModeRequired();
    void homeModeRequired();

protected:
    virtual void onExit(QEvent *event);
    virtual void onEntry(QEvent *event);

    /**
     * This function should be called on entry of each state to show
     * the current Background States Icons.
     * Each state should be able to add itself to the backgroundStates List if a background mode
     * makes sense. This list is used to draw icons to nackground states.
     */
    void showBackgroundStates();

protected:
    MediaCenter::Browser *m_browser;
    MediaCenter::Player *m_player;
    MediaCenter::PlaybackControl *m_control;
    MediaCenter::Playlist *m_playlist;
    MediaCenter::InfoDisplay *m_infoDisplay;
    MediaCenter::HomeApplet *m_home;
    MediaCenter::MediaLayout *m_layout;

private:
    static MediaCenter::Mode s_currentState;

protected Q_SLOTS:
    virtual void onPlaybackStateChanged(const MediaCenter::PlaybackState &state);

private Q_SLOTS:
    void slotSetCurrentState(const MediaCenter::Mode &mode);
    void modeChangeRequired(MediaCenter::Mode);

};

} //namespace MediaCenter
#endif // MEDIACENTERSTATE_H
