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
#include <QPixmap>

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
    void slotFinished();

private:
    QPixmap m_picture;
    qint64 m_showTime;
    QTimer *m_timer;
    QRectF m_pictureRect;
    QString m_picturePath;
    QRect m_relativePictureRect;

private:
    void adjustPixmapSize(const QSize &);
};

#endif // PICTUREVIEWER_H
