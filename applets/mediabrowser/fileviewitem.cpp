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

#include "fileviewitem.h"

#include <KFileItem>
#include <KDirModel>

#include <QStyle>
#include <QStyleOptionViewItemV4>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include <kio/job.h>
#include <kio/previewjob.h>


FileViewItem::FileViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent) :
    MediaCenter::ViewItem(option, parent),
    m_pressPoint(QPoint(-1, -1))
{
}

void FileViewItem::setItemType(ItemType type)
{
    m_type = type;
}

FileViewItem::ItemType FileViewItem::itemType() const
{
    return m_type;
}

void FileViewItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    ViewItem::paint(painter, option, widget);
    if (styleOption().state & QStyle::State_MouseOver) {
        drawSelectionIcon(painter, option);
    }
}

void FileViewItem::setModelIndex(const QModelIndex &index)
{
    ViewItem::setModelIndex(index);
    askForFilePreview();
}

void FileViewItem::drawSelectionIcon(QPainter* painter, const QStyleOptionGraphicsItem* option)
{
    const char* icon = (styleOption().state & QStyle::State_Selected) ? "list-remove" : "list-add";
    QPixmap selectIcon = KIconLoader::global()->loadIcon(icon, KIconLoader::NoGroup, qMin(16,16));
    painter->drawPixmap(selectionIconRect(option->rect), selectIcon);
}

void FileViewItem::askForFilePreview()
{
    // if the model is a KDirModel ask for pixmaps
    QVariant data = modelIndex().data(KDirModel::FileItemRole);
    if (data.isNull()) {
        return;
    }

    KFileItem item = modelIndex().data(KDirModel::FileItemRole).value<KFileItem>();

    bool ok;
    KUrl url = item.mostLocalUrl(ok);
    if (!ok) {
        return;
    }
    KIO::PreviewJob *pixmapJob = KIO::filePreview(url, decorationSize(), 0, 0, 0, false, true, 0);
    connect(pixmapJob, SIGNAL(gotPreview(const KFileItem &, const QPixmap &)), this, SLOT(slotGotPreview(const KFileItem &, const QPixmap&)));

}

void FileViewItem::slotGotPreview(const KFileItem &item, const QPixmap &pixmap)
{
    Q_UNUSED(item);
    setPixmap(pixmap);
    update();
}

QRect FileViewItem::selectionIconRect(const QRect &contentsRect) const
{
    QRect selectRect(0, 0, 32, 32);
    selectRect.moveTo(contentsRect.right() - selectRect.width() - 5, contentsRect.bottom() - selectRect.height() - 5);

    return selectRect;
}

bool FileViewItem::itemSelected() const
{
    return styleOption().state.testFlag(QStyle::State_Selected);
}

void FileViewItem::setItemSelected(bool set)
{
    QStyleOptionViewItemV4 option = styleOption();

    if (option.state.testFlag(QStyle::State_Selected) == set) {
        return;
    }
    set ? option.state |= QStyle::State_Selected : option.state ^= QStyle::State_Selected;
}

void FileViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectionIconRect(contentsRect().toRect()).contains(event->pos().toPoint())) {
        m_pressPoint = event->pos().toPoint();
    }
}

void FileViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    const QRect selectionIconRect = this->selectionIconRect(contentsRect().toRect());
    if (selectionIconRect.contains(event->pos().toPoint()) && selectionIconRect.contains(m_pressPoint)) {
        emit selectionEmblemToggled();
    }
}
