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
#ifndef MEDIALAYOUT_H
#define MEDIALAYOUT_H

#include <QObject>
#include <QRectF>

namespace Plasma {
    class Containment;
    class Applet;
}

class QGraphicsItem;

/**
 * @class MediaLayout
 * @brief MediaLayout takes care of layouting media applets.
 * MediaLayout takes care of layouting media applets in the containment.
 * It also takes care of showing proper applet handles when needed.
 * @author Alessandro Diaferia
 */

class MediaLayout : public QObject
{
    Q_OBJECT
public:
    MediaLayout(Plasma::Containment *parent);
    ~MediaLayout();

    void setBrowser(Plasma::Applet *browser);
    void setPlaybackControl(Plasma::Applet *control);
    void setPlaylist(Plasma::Applet *playlist);
    void setPlayer(Plasma::Applet *player);

    /**
     * Calling this function makes the layout relayouting
     * applets in the containment. It is reccommended to
     * call this function whenever setting new applets.
     */
    void invalidate();

    bool eventFilter(QObject *o, QEvent *e);

    QRectF browserPreferredShowingRect() const;
    QRectF controllerPreferredShowingRect() const;
    QRectF playlistPreferredShowingRect() const;

protected slots:
    void animateShowingApplet(Plasma::Applet *);
    void animateHidingApplet(Plasma::Applet *);

private:
    Plasma::Containment *m_containment;
    Plasma::Applet *m_browser;
    Plasma::Applet *m_control;
    Plasma::Applet *m_playlist;
    Plasma::Applet *m_player;

    QList<Plasma::Applet*> m_needLayouting;

private:
    void doCompleteLayout();

    void layoutBrowser();
    void layoutControl();
    void layoutPlaylist();
    void layoutPlayer();

private slots:
    void restoreHandlerGlows(QGraphicsItem*);
};

#endif // MEDIALAYOUT_H
