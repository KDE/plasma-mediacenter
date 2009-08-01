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
#include "treeview.h"

#include <QMouseEvent>
#include <QStyleOptionViewItem>

#include <Plasma/Theme>

#include <KDebug>
#include <KGlobalSettings>

TreeView::TreeView(QWidget *parent) : QTreeView(parent),
m_pressed(false)
{
    setAttribute(Qt::WA_NoSystemBackground);
    setFrameStyle(QFrame::NoFrame);

    setHeaderHidden(true);
    setRootIsDecorated(false);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::transparent);
    p.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    setPalette(p);

    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(updateColors()));
}

TreeView::~TreeView()
{}

void TreeView::updateColors()
{
    QPalette p = palette();
    p.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    setPalette(p);
}

void TreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (KGlobalSettings::singleClick()) {
        return;
    }
    emit activated(indexAt(event->pos()));
}

void TreeView::mousePressEvent(QMouseEvent *event)
{
//    Q_UNUSED(event);
    QStyleOptionViewItem option = viewOptions();
    QRect rect = visualRect(indexAt(event->pos()));
    option.rect = rect;
    if (itemDelegate()->editorEvent(event, model(), option, indexAt(event->pos()))) {
        return;
    }
    m_pressed = true;
}

void TreeView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressed && KGlobalSettings::singleClick()) {
        QModelIndex index = indexAt(event->pos());
        if (index.isValid()) {
            emit activated(indexAt(event->pos()));
        }
    }
}
