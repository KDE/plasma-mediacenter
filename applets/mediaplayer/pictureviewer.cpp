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

#include <KDebug>

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
    update();
}

void PictureViewer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    if (!m_picture) {
        return;
    }

    QImage picture = *m_picture;
    painter->fillRect(option->rect, Qt::black);
    // the image is bigger than the contents rect
    if (m_picture->height() > (int)option->rect.height() && m_picture->width() > (int)option->rect.width()) {
        picture = m_picture->scaled(option->rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    QRectF rect;
    rect.setX((option->rect.width() - picture.width()) / 2.0);
    rect.setY((option->rect.height() - picture.height()) / 2.0);
    rect.setWidth(picture.width());
    rect.setHeight(picture.height());
    painter->drawImage(rect, picture);

    painter->drawImage(rect, picture);
}
