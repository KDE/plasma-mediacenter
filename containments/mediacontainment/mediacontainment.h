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
#ifndef MEDIACONTAINMENT_H
#define MEDIACONTAINMENT_H

#include <Plasma/Containment>
#include <QList>

class QAction;
class QPointF;
namespace MediaCenter {
    class Browser;
    class PlaybackControl;
    class Playlist;
    class Player;
}

class MediaLayout;

class MediaContainment : public Plasma::Containment
{
    Q_OBJECT
public:
    MediaContainment(QObject *parent, const QVariantList &args);
    ~MediaContainment();

    QList<QAction*> contextualActions();

protected:
    void constraintsEvent(Plasma::Constraints constraints);

private slots:
    void slotAppletAdded(Plasma::Applet *applet, const QPointF &pos);
    void slotAppletRemoved(Plasma::Applet *applet);

private:
    MediaCenter::Browser *m_browser;
    MediaCenter::PlaybackControl *m_control;
    MediaCenter::Playlist *m_playlist;
    MediaCenter::Player *m_player;

    MediaLayout *m_layout;

    void addMediaApplet(Plasma::Applet *);
};

#endif // MEDIACONTAINMENT_H
