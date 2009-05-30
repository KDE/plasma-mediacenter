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
#include "abstractmediaitemview.h"

// Plasma
#include <Plasma/ScrollBar>
#include <Plasma/Theme>

// KDE
#include <KIconLoader>
#include <KFileItemDelegate>
#include <KDebug>

// Qt
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QLocale>

AbstractMediaItemView::AbstractMediaItemView(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_model(0),
m_delegate(0),
m_scrollBar(new Plasma::ScrollBar(this))

{
    setIconSize(KIconLoader::global()->currentSize(KIconLoader::Desktop));
    connect (m_scrollBar, SIGNAL(valueChanged(int)), this, SIGNAL(scrollOffsetChanged(int)));
}

AbstractMediaItemView::~AbstractMediaItemView()
{}

void AbstractMediaItemView::setIconSize(int size)
{
    m_iconSize = size;
    update();
}

int AbstractMediaItemView::iconSize() const
{
    return m_iconSize;
}

void AbstractMediaItemView::setModel(QAbstractItemModel *model)
{
    if (m_model) {
        delete m_model;
    }

    m_model = model;
}

QAbstractItemModel* AbstractMediaItemView::model()
{
    return m_model;
}

void AbstractMediaItemView::setItemDelegate(QAbstractItemDelegate *delegate)
{
    if (m_delegate) {
        delete m_delegate;
    }

    m_delegate = delegate;
}

QAbstractItemDelegate* AbstractMediaItemView::itemDelegate()
{
    return m_delegate;
}

QScrollBar* AbstractMediaItemView::verticalScrollBar()
{
    return m_scrollBar->nativeWidget();
}

QRect AbstractMediaItemView::contentsArea() const
{
    if (!m_scrollBar->isVisible()) {
        return contentsRect().toRect();
    }
    QRect contents = contentsRect().toRect();
    contents.setWidth(contents.width() - m_scrollBar->nativeWidget()->width());

    return contents;
}

void AbstractMediaItemView::hideVerticalScrollBar()
{
    m_scrollBar->hide();
}

bool AbstractMediaItemView::isVerticalScrollBarHidden()
{
    return !m_scrollBar->isVisible();
}

void AbstractMediaItemView::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    m_scrollBar->resize(m_scrollBar->size().width(), contentsRect().height());
    m_scrollBar->setPos(contentsRect().width() - m_scrollBar->size().width(), 0);
}

void AbstractMediaItemView::setupOptions()
{
    m_option.palette.setColor(QPalette::Text, Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    m_option.font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DesktopFont);
    m_option.fontMetrics = QFontMetrics(m_option.font);
    m_option.decorationSize = QSize(iconSize(), iconSize());
    m_option.locale = QLocale::system();
    m_option.widget = 0;
    m_option.state |= QStyle::State_Enabled;
}

void AbstractMediaItemView::setRootIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

QModelIndex AbstractMediaItemView::rootIndex() const
{
    return m_rootIndex;
}

void AbstractMediaItemView::updateScrollBar()
{
    verticalScrollBar()->setRange(0, m_model->rowCount(m_rootIndex));
    kDebug() << verticalScrollBar()->maximum();
    verticalScrollBar()->setSingleStep(1);
}
