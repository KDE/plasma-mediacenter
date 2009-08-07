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
#include "pictureviewer.h"

#include <mediacenter/mediacenter.h>

#include <QPainter>
#include <QStyleOptionGraphicsItem>

PictureViewer::PictureViewer(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_picture(0)
{
}

PictureViewer::~PictureViewer()
{}

void PictureViewer::loadPicture(const QString &path)
{
    MediaCenter::MediaType type = MediaCenter::getType(path);
    if (type != MediaCenter::Picture) {
        delete m_picture;
        m_picture = 0;
        return;
    }

    m_picture = new QImage(path);
}

void PictureViewer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    if (!m_picture) {
        return;
    }

    painter->fillRect(option->rect, Qt::black);
    // the image is smaller than the contents rect
    if (m_picture->height() <= (int)option->rect.height() && m_picture->width() <= (int)option->rect.width()) {
        QRectF rect;
        rect.setX((option->rect.width() - m_picture->width()) / 2.0);
        rect.setY((option->rect.height() - m_picture->height()) / 2.0);
        rect.setWidth(m_picture->width());
        rect.setHeight(m_picture->height());
        painter->drawImage(rect, *m_picture);
        return;
    }

    QRectF rect;
    if (m_picture->height() > (int)option->rect.height() && m_picture->height() >= m_picture->width()) {
        qreal hRatio = option->rect.height() / m_picture->height();
        rect.setHeight(option->rect.height());
        rect.setWidth((qreal)m_picture->width() * hRatio);
    } else if (m_picture->width() > (int)option->rect.width() && m_picture->width() >= m_picture->height()) {
        qreal wRatio = option->rect.width() / m_picture->width();
        rect.setHeight((qreal)m_picture->height() * wRatio);
        rect.setWidth(option->rect.width());
    }

    rect.setX((option->rect.width() - rect.width()) / 2);
    rect.setY((option->rect.height() - rect.height()) / 2);

    painter->drawImage(rect, *m_picture);

}
