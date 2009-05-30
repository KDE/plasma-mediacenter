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
#include "listview.h"

// Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QDir>
#include <QScrollBar>

// KDE
#include <KDirModel>
#include <KDirLister>
#include <KUrl>
#include <KFileItemDelegate>
#include <KDebug>

ListView::ListView(QGraphicsItem *parent) : AbstractMediaItemView(parent)
{
    setupOptions();
    switchToFileModel();
    connect (this, SIGNAL(scrollOffsetChanged(int)), this, SLOT(scrollView(int)));
}

ListView::~ListView()
{}

void ListView::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    if (!m_delegate) {
        return;
    }

    painter->setClipRect(contentsArea());

    // scrolling code;
    painter->translate(0, -verticalScrollBar()->value() * iconSize() * 2);

    QRect item(contentsArea().x(), contentsArea().y(), contentsArea().width(), iconSize() * 2);
    kDebug() << m_model->rowCount();
    for (int i = 0; i < m_model->rowCount(); i++) {
        m_option.rect = item;
        m_delegate->paint(painter, m_option, m_model->index(i, 0, QModelIndex()));
        item.translate(0, iconSize() * 2);
    }
}

void ListView::setupOptions()
{
    AbstractMediaItemView::setupOptions();
    m_option.decorationPosition = QStyleOptionViewItem::Left;
    m_option.decorationAlignment = Qt::AlignCenter;
    m_option.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
}

void ListView::switchToFileModel()
{
    KDirModel *model = new KDirModel(this);
    KDirLister *lister = new KDirLister(this);
    connect (lister, SIGNAL(completed()), this, SLOT(updateScrollBar()));
    model->setDirLister(lister);
    lister->openUrl(KUrl(QDir::homePath()));
    m_model = model;
    m_delegate = new KFileItemDelegate(this);


    update();
}

void ListView::scrollView(int value)
{
    kDebug() << verticalScrollBar()->value();
    update();
}
