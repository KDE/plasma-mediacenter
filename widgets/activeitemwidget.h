/*
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef ACTIVEITEMWIDGET_H
#define ACTIVEITEMWIDGET_H

#include <QGraphicsWidget>

class QIcon;
class QVariant;

class ActiveItemWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    ActiveItemWidget(QGraphicsItem *parent = 0);
    ~ActiveItemWidget();

    void addItem(const QString &text);
    void addItem(const QIcon &icon, const QString &text);
    void addItems(const QStringList &items);

    int count() const;
    int activeItemIndex() const;

    void setIconSize(const QSize &size);
    QSize iconSize() const;

    QIcon itemIcon(int index) const;
    QString itemText(int index) const;
    QVariant itemData(int index, int role = Qt::UserRole) const;

    QIcon activeItemIcon() const;
    QString activeItemText() const;
    QVariant activeItemData(int role = Qt::UserRole) const;

    void setItemIcon(int index, const QIcon &icon);
    void setItemText(int index, const QString &text);
    void setItemData(int index, const QVariant &value, int role = Qt::UserRole);

public Q_SLOTS:
    void next();
    void previous();
    void setActiveItem(int index);
    void setActiveItemText(const QString &text);
    void setActiveItemIcon(const QIcon &icon);
    void setActiveItemData(const QVariant &value, int role = Qt::UserRole);

private:
    class ActiveItemWidgetPrivate;
    ActiveItemWidgetPrivate *d;

    typedef struct Item {
        QString text;
        QIcon icon;
        QHash<int, QVariant> data;
    } Item;

};

#endif
