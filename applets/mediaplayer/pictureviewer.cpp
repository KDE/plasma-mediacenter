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
#include <QTimer>

#include <KDebug>

PictureViewer::PictureViewer(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_picture(QPixmap()),
m_showTime(3),
m_pictureRect(QRectF()),
m_timer(new QTimer(this)),
m_picturePath(QString())
{
    m_timer->setSingleShot(true);
    connect (m_timer, SIGNAL(timeout()), this, SLOT(slotFinished()));
}

PictureViewer::~PictureViewer()
{}

void PictureViewer::loadPicture(const QString &path)
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    if (m_picturePath == path) {
        return;
    }

    MediaCenter::MediaType type = MediaCenter::getType(path);
    if (type != MediaCenter::Picture) {
        return;
    }
    m_picturePath = path;
    m_picture = QPixmap(m_picturePath);
    m_pictureRect = QRectF(0,0, m_picture.width(), m_picture.height());

    adjustPixmapSize(size().toSize());
    update();
}

void PictureViewer::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    QGraphicsWidget::resizeEvent(event);
    kDebug() << "resized to size" << size().toSize();
    adjustPixmapSize(size().toSize());
}

void PictureViewer::adjustPixmapSize(const QSize &s)
{
    if (s.isNull()) {
        return;
    }
    QPixmap picture = QPixmap(m_picturePath);
    // if the original pixmap fits the widget (aka is smaller than needed)
    // then we shouldn't change its size. We only want to calculate the needed
    // rect
    if (picture.width() < s.width() && picture.height() < s.height()) {
        QRect rect;
        rect.setX((s.width() - picture.width()) / 2.0);
        rect.setY((s.height() - picture.height()) / 2.0);
        rect.setWidth(picture.width());
        rect.setHeight(picture.height());
        m_relativePictureRect = rect;

        m_picture = picture;
        return;
    }

    // the image is bigger than the contents rect
    picture = picture.scaled(s, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_picture = picture;

    QRect rect;
    rect.setX((s.width() - m_picture.width()) / 2.0);
    rect.setY((s.height() - m_picture.height()) / 2.0);
    rect.setWidth(m_picture.width());
    rect.setHeight(m_picture.height());
    m_relativePictureRect = rect;
}

void PictureViewer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->fillRect(option->rect, Qt::black);

    if (m_picture.isNull()) {
        kDebug() << "null pixmap; returning from paint";
        return;
    }

    painter->drawPixmap(m_relativePictureRect.topLeft(), m_picture);
}

void PictureViewer::setShowTime(qint64 time)
{
    m_showTime = time;
}

int PictureViewer::showTime() const
{
    return m_showTime;
}

void PictureViewer::startTimer()
{
    if (m_showTime) {
        m_timer->start(m_showTime*1000);
    }
}

void PictureViewer::stopTimer()
{
    m_timer->stop();
}

void PictureViewer::clearImage()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    m_picture = QPixmap();
    update();
}

void PictureViewer::slotFinished()
{
    m_picture = QPixmap();
    update();
    emit showFinished();
}

bool PictureViewer::isTimerActive() const
{
    return m_timer->isActive();
}

QRectF PictureViewer::pictureRect() const
{
    return m_pictureRect;
}

