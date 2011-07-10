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
#ifndef MEDIANOTIFICATIONWIDGET_H
#define MEDIANOTIFICATIONWIDGET_H

#include <QGraphicsWidget>
namespace Plasma {
    class Applet;
}

class MediaNotificationWidget : public QGraphicsWidget
{
    Q_OBJECT
    Q_PROPERTY(QString notificationTitle READ notificationTitle WRITE setNotificationTitle)
public:
    MediaNotificationWidget(Plasma::Applet *parent);
    ~MediaNotificationWidget();

    void setNotificationTitle(const QString &title);
    QString notificationTitle() const;

    void setNotificationText(const QString &text);
    QString notificationText() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /**
     * Shows the notification using a nice animation.
     * @param applet is the container for the notification widget
     * which could be either an applet or a containment.
     */
    static void showNotification(Plasma::Applet *applet, const QString &text, const QString &title = QString());

private:
    QString m_title;
    QString m_text;
    Plasma::Applet *m_parent;
};

#endif // MEDIANOTIFICATIONWIDGET_H
