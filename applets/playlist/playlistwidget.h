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
#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QGraphicsWidget>

namespace Plasma {
    class TreeView;
    class DataEngine;
}
class QStandardItemModel;
class QDBusInterface;

class PlaylistWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    PlaylistWidget(QGraphicsItem *parent = 0);
    ~PlaylistWidget();

protected slots:
    void jobCompleted();
    void showPlaylist(const QString &playlistName);

private:
    Plasma::TreeView *m_treeView;
    Plasma::DataEngine *m_playlistEngine;
    QStandardItemModel *m_model;
    QDBusInterface *m_interface;
};

#endif
