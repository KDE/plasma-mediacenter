/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef MEDIALAYOUT_H
#define MEDIALAYOUT_H

#include "mediacenter.h"
#include "mediacenter_export.h"

#include <QObject>
#include <QRectF>

#include <Plasma/Applet>
#include <plasma/animations/animation.h>

namespace Plasma {
    class Containment;
}
class MediaHandler;
class QGraphicsItem;

namespace MediaCenter {


/**
 * @class MediaLayout
 * @brief MediaLayout takes care of layouting media applets.
 * MediaLayout takes care of layouting media applets in the containment.
 * It also takes care of showing proper applet handles when needed.
 * @author Alessandro Diaferia
 */

class MEDIACENTER_EXPORT MediaLayout : public QObject
{
    Q_OBJECT
public:
    MediaLayout(Plasma::Containment *parent);
    ~MediaLayout();

    void setBrowser(Plasma::Applet *browser);
    void setPlaybackControl(Plasma::Applet *control);
    void setPlaylist(Plasma::Applet *playlist);
    void setPlayer(Plasma::Applet *player);
    void setInfoDisplay(Plasma::Applet *infoDisplay);

    /**
     * Calling this function makes the layout relayouting
     * applets in the containment. It is reccommended to
     * call this function whenever setting new applets.
     */
    void invalidate();
    void layoutPlayer(); //States need to call this sometimes

    bool eventFilter(QObject *o, QEvent *e);

    void setPlaylistVisible(bool set);
    bool playlistVisible();

    void setInfoDisplayOnly(bool set);

    QRectF browserPreferredShowingRect() const;
    QRectF controllerPreferredShowingRect() const;
    QRectF playlistPreferredShowingRect() const;
    QRectF infoDisplayPreferredShowingRect() const;

    MediaCenter::InfoDisplayMode infoDisplayMode() const;

public slots:
    void toggleShowAllMediaApplets();

    void togglePlaylistVisible();
    void toggleControlAutohide();
    void setControlAutohide(bool);
    void controlAutohideOn();
    void controlAutohideOff();

    void showBrowser();
    void showPlayer();
    void showPlaylist();
    void hidePlaylist();

    void setInfoDisplayMode(const MediaCenter::InfoDisplayMode &mode);

protected slots:
    void animateShowingApplet(Plasma::Applet *);
    void animateHidingApplet(Plasma::Applet *);

private:
    Plasma::Containment *m_containment;
    Plasma::Applet *m_browser;
    Plasma::Applet *m_control;
    Plasma::Applet *m_playlist;
    Plasma::Applet *m_player;
    Plasma::Applet *m_infoDisplay;

    Plasma::Applet::BackgroundHints m_browserBackgroundHints;

    QList<Plasma::Applet*> m_needLayouting;

    bool m_showAll;
    bool m_playlistVisible;
    bool m_controlAutohide;
    bool m_controlVisible;
    bool m_infoDisplayOnly;

    MediaHandler* m_playlistHandler;
    MediaHandler* m_controlHandler;
    MediaHandler* m_infoDisplayHandler;

    MediaCenter::InfoDisplayMode m_infoDisplayMode;

    Plasma::Animation *m_resizeBrowserAnimation;
    Plasma::Animation *m_resizePlaylistAnimation;
    Plasma::Animation *m_resizePlayerAnimation;
    Plasma::Animation *m_resizeInfoDisplayAnimation;

    Plasma::Animation *m_slidePlaylistAnimation;
    Plasma::Animation *m_slideControlAnimation;
    Plasma::Animation *m_slideInfoDisplayAnimation;

    Plasma::Animation *m_fadeOutPlayer;
    Plasma::Animation *m_fadeInPlayer;

    QRectF m_playerNewRect;

private:
    void doCompleteLayout();
    void layoutBrowser();
    void layoutControl();
    void layoutInfoDisplay();
    void layoutPlaylist();

    void animateResizingApplet(Plasma::Applet *applet, QRectF target);

private slots:
    void setEnableGlowHandler(QGraphicsItem*, bool set);
    void fadeInPlayer();
};
} //ending namespace MEDIACENTER
#endif // MEDIALAYOUT_H
