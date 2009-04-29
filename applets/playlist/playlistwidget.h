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
#include <Plasma/DataEngine>

namespace Plasma {
    class TreeView;
    class ComboBox;
}

class QStandardItemModel;
class QDBusInterface;
class QGraphicsSceneDragDropEvent;
class QEvent;

class PlaylistWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    PlaylistWidget(QGraphicsItem *parent = 0);
    ~PlaylistWidget();

public slots:
    void dataUpdated(const QString &, const Plasma::DataEngine::Data &);

protected slots:
    void slotPlaylistAdded(const QString &source);
    void slotCoverAdded(const QString &source);
    void showPlaylist(const QString &playlistName);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    enum ConvenientRoles {
        CoverSourceRole = Qt::UserRole + 4
    };

    Plasma::TreeView *m_treeView;
    Plasma::ComboBox *m_comboBox;
    Plasma::DataEngine *m_playlistEngine;
    Plasma::DataEngine *m_coverEngine;
    QStandardItemModel *m_model;
    QDBusInterface *m_interface;
};

#endif
