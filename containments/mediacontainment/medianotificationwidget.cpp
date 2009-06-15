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
#include "medianotificationwidget.h"

// Plasma
#include <Plasma/Applet>
#include <Plasma/Theme>
#include <Plasma/Animator>

// Qt
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QColor>

MediaNotificationWidget::MediaNotificationWidget(Plasma::Applet *parent) : QGraphicsWidget(parent)
{
}

MediaNotificationWidget::~MediaNotificationWidget()
{}

void MediaNotificationWidget::setNotificationText(const QString &text)
{
    m_text = text;
    update();
}

QString MediaNotificationWidget::notificationText() const
{
    return m_text;
}

void MediaNotificationWidget::setNotificationTitle(const QString &title)
{
    m_title = title;
    update();
}

QString MediaNotificationWidget::notificationTitle() const
{
    return m_title;
}

void MediaNotificationWidget::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QColor color(37,37, 37, 211);
    painter->fillRect(option->rect, color);

    QFont font = Plasma::Theme::defaultTheme()->font(Plasma::Theme::DesktopFont);

    const QRect titleRect(option->rect.left(), option->rect.top(), option->rect.width(), Plasma::Theme::defaultTheme()->fontMetrics().height());
    painter->drawText(titleRect, m_title);

    const QRect textRect(titleRect.x(), titleRect.bottom() + 5, option->rect.width(), option->rect.height() - titleRect.height() - 5);
    painter->drawText(textRect, m_text);
}

void MediaNotificationWidget::showNotification(Plasma::Applet *applet, const QString &text, const QString &title)
{
    MediaNotificationWidget *notifier = new MediaNotificationWidget(applet);
    notifier->setNotificationText(text);
    notifier->setNotificationTitle(title);

    notifier->resize(300, 300);
    notifier->setPos(applet->rect().center().x() - 150, applet->rect().center().y() - 150);

    Plasma::Animator::self()->animateItem(notifier, Plasma::Animator::AppearAnimation);

}
