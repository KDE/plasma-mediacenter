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
#ifndef PICTUREVIEWER_H
#define PICTUREVIEWER_H

#include <QGraphicsWidget>
#include <QRunnable>
#include <QTimer>

class QGraphicsSceneResizeEvent;

class PictureViewer : public QGraphicsWidget
{
    Q_OBJECT
public:
    PictureViewer(QGraphicsItem *parent = 0);
    ~PictureViewer();

    void loadPicture(const QString &path);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int showTime() const;

    bool isTimerActive() const;

    QRectF pictureRect() const;

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

public Q_SLOTS:
    /**
     * Call this method in order to set the slideshow time.
     * It is set to 3 secs by default. 0 means no slideshow or
     * infinite time.
     */
    void setShowTime(qint64 time);

    void startTimer();
    void stopTimer();
    void clearImage();

Q_SIGNALS:
    void showFinished();

private Q_SLOTS:
    void slotImageLoaded(const QImage &image);
    void slotImageScaled(const QSize &size, const QImage &image);
    void slotDelayedResize();

private:
    QImage m_picture;
    QImage m_scaledPicture;
    qint64 m_showTime;
    QTimer m_timer;
    QRectF m_pictureRect;
    QString m_picturePath;
    QRect m_relativePictureRect;
    QTimer m_resizeTimer;

private:
    void adjustPixmapSize(const QSize &);
};

//// helper classes ////

class ImageLoader : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ImageLoader(const QString &imagePath)
    {
        m_path = imagePath;
    }

    void run()
    {
        QImage image(m_path);
        emit loaded(image);
    }

signals:
    void loaded(const QImage &image);

private:
    QString m_path;
};

class ImageScaler : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ImageScaler(const QImage &image, const QSize &newSize)
    {
        m_image = image;
        m_size = newSize;
    }

    void run()
    {
        QImage image = m_image.scaled(m_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        emit scaled(m_size, image);
    }

signals:
    void scaled(const QSize &size, const QImage &image);

private:
    QImage m_image;
    QSize m_size;
};

#endif // PICTUREVIEWER_H
