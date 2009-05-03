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
#ifndef PLAYLISTDELEGATE_H
#define PLAYLISTDELEGATE_H

#include <QStyledItemDelegate>

namespace Plasma {
    class FrameSvg;
}

class PlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    enum PlaylistRoles {
        AuthorRole = Qt::UserRole + 1,
        AlbumRole = Qt::UserRole + 2,
        TrackNameRole = Qt::DisplayRole,
        CoverRole = Qt::DecorationRole
        };

    PlaylistDelegate(QObject *parent = 0);
    ~PlaylistDelegate();

    void paint (QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent (QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    QSize sizeHint (const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    Plasma::FrameSvg *m_frameSvg;
};

#endif // PLAYLISTDELEGATE_H
