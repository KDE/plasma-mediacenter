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
#include <QThreadPool>

#include <KDebug>

PictureViewer::PictureViewer(QGraphicsItem *parent) : QGraphicsWidget(parent),
m_picture(QImage()),
m_showTime(3),
m_pictureRect(QRectF()),
m_picturePath(QString()),
m_delayedSmoothScaling(false),
m_emitImageLoaded(false)
{
    m_timer.setSingleShot(true);
    connect (&m_timer, SIGNAL(timeout()), this, SIGNAL(showFinished()));

    m_resizeTimer.setSingleShot(true);
    connect(&m_resizeTimer, SIGNAL(timeout()), this, SLOT(slotDelayedResize()));
}

PictureViewer::~PictureViewer()
{}

void PictureViewer::loadPicture(const QString &path)
{
    if (m_timer.isActive()) {
        m_timer.stop();
    }

    if (m_picturePath == path) {
        return;
    }

    MediaCenter::MediaType type = MediaCenter::getType(path);
    if (type != MediaCenter::Picture) {
        return;
    }
    m_emitImageLoaded = true;
    m_picturePath = path;

    ImageLoader *loader = new ImageLoader(m_picturePath);
    connect(loader, SIGNAL(loaded(QImage)), this, SLOT(slotImageLoaded(QImage)));
    QThreadPool::globalInstance()->start(loader);
}

void PictureViewer::slotImageLoaded(const QImage &image)
{
    m_picture = image;
    m_scaledPicture = QImage();
    m_pictureRect = QRectF(0,0, m_picture.width(), m_picture.height());
    m_relativePictureRect = QRect();
    adjustPixmapSize(size().toSize());
}

void PictureViewer::slotImageScaled(const QSize &s, const QImage& image)
{
    m_scaledPicture = image;

    QRect rect;
    rect.setX((size().width() - m_scaledPicture.width()) / 2.0);
    rect.setY((size().height() - m_scaledPicture.height()) / 2.0);
    rect.setWidth(m_scaledPicture.width());
    rect.setHeight(m_scaledPicture.height());
    m_relativePictureRect = rect;

    update();
    if (m_emitImageLoaded) {
        emit imageLoaded();
        m_emitImageLoaded = false;
    }
}

void PictureViewer::slotDelayedResize()
{
    m_delayedSmoothScaling = false;
    adjustPixmapSize(size().toSize());
}

void PictureViewer::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    QGraphicsWidget::resizeEvent(event);

    if (m_resizeTimer.isActive()) {
        m_resizeTimer.stop();
    }
    m_delayedSmoothScaling = true;
    m_resizeTimer.start(200);
    adjustPixmapSize(size().toSize());
}

void PictureViewer::adjustPixmapSize(const QSize &s)
{
    if (s.isNull()) {
        return;
    }

    const QSize originalSize = m_picture.size();
    // if the original pixmap fits the widget (aka is smaller than needed)
    // then we shouldn't change its size. We only want to calculate the needed
    // rect
    if (originalSize.width() < s.width() && originalSize.height() < s.height()) {
        QRect rect;
        rect.setX((s.width() - originalSize.width()) / 2.0);
        rect.setY((s.height() - originalSize.height()) / 2.0);
        rect.setWidth(originalSize.width());
        rect.setHeight(originalSize.height());
        m_relativePictureRect = rect;

        return;
    }

    // the image is bigger than the contents rect
    if (!m_delayedSmoothScaling) {
        ImageScaler *scaler = new ImageScaler(m_picture, s);
        connect(scaler, SIGNAL(scaled(QSize,QImage)), this, SLOT(slotImageScaled(QSize, QImage)));
        QThreadPool::globalInstance()->start(scaler);
    } else {
        slotImageScaled(s, m_picture.scaled(s, Qt::KeepAspectRatio));
    }
}

void PictureViewer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    if (m_picture.isNull()) {
        kDebug() << "null pixmap; returning from paint";
        return;
    }
    painter->fillRect(option->rect, Qt::black);

    QImage picture = m_scaledPicture.isNull() ? m_picture : m_scaledPicture;
    painter->drawImage(m_relativePictureRect.topLeft(), picture);
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
        m_timer.start(m_showTime*1000);
    }
}

void PictureViewer::stopTimer()
{
    m_timer.stop();
}

void PictureViewer::clearImage()
{
    if (m_timer.isActive()) {
        m_timer.stop();
    }

    m_picture = QImage();
    m_picturePath = QString();
    m_scaledPicture = QImage();
    update();
}

bool PictureViewer::isTimerActive() const
{
    return m_timer.isActive();
}

QRectF PictureViewer::pictureRect() const
{
    return m_pictureRect;
}

