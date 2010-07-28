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

#ifndef FILEVIEWITEM_H
#define FILEVIEWITEM_H

#include <mediacenter/widgets/viewitem.h>

class KFileItem;

class FileViewItem : public MediaCenter::ViewItem
{
    Q_OBJECT
public:
    enum ItemType {GenericItem, LocalFileItem, RemoteFileItem};

    FileViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent = 0);

    void setItemType(ItemType);
    ItemType itemType() const;

    bool itemSelected() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setModelIndex(const QModelIndex &index);

public slots:
    void setItemSelected(bool set);

private:
    ItemType m_type;
    QPoint m_pressPoint;

private:
    void drawSelectionIcon(QPainter *painter, const QStyleOptionGraphicsItem *option);
    QRect selectionIconRect(const QRect &contentsRect) const;
    void askForFilePreview();

private slots:
    void slotGotPreview(const KFileItem &item, const QPixmap &preview);

signals:
    void selectionEmblemToggled();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


};

#endif // FILEVIEWITEM_H
