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

#ifndef VIEWITEM_H
#define VIEWITEM_H

#include <QGraphicsWidget>
#include <QStyleOptionViewItemV4>
#include <QModelIndex>

#include "mediacenter/mediacenter_export.h"

namespace Plasma {
    class FrameSvg;
}

class QPixmap;

namespace MediaCenter {

class MEDIACENTER_EXPORT ViewItem : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon);
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap);
    Q_PROPERTY(QString text READ text WRITE setText);
    Q_PROPERTY(int decorationSize READ decorationSize WRITE setDecorationSize);
    Q_PROPERTY(QModelIndex modelIndex READ modelIndex WRITE setModelIndex);
    Q_PROPERTY(QSize itemSizeHint READ itemSizeHint);
public:
    /**
     * Each ViewItem must be created passing the option object
     * describing its appearance constraints.
     */
    ViewItem(QGraphicsItem *parent = 0);
    ViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent = 0);
    ~ViewItem();

    virtual void setStyleOption(const QStyleOptionViewItemV4 &);
    QStyleOptionViewItemV4 styleOption() const;

    virtual void setModelIndex(const QModelIndex &index);
    QModelIndex modelIndex() const;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QSize itemSizeHint() const;

    virtual void setDrawBlurredText(bool set);
    bool drawBlurredText();

    /**
     * @return the icon set with the setIcon function or a
     * null icon if none was set.
     */
    QIcon icon() const;

    QPixmap pixmap() const;

    /**
     * Overrides the text passed through the option parameter
     * in the paint method with the @param text
     * @note This is particularly useful when accessing
     * the icon through the property system.
     */
    void setText(const QString &text);

    /**
     * @return the text set with the setText function
     */
    QString text() const;

    /**
     * Sets the decoration size for the item. If setStyleOption is called
     * after this method the decorationSize specified in the given @param option
     * will override this setting and vice versa.
     */
    void setDecorationSize(int size);

    /**
     * @return the currently used decoration size.
     */
    int decorationSize() const;

public slots:
    void setPixmap(const QPixmap &pixmap);

    /**
     * This method allows setting a custom icon to be shown
     * in place of the one passed by the index as
     * parameter of the paint function.
     * @note This is particularly useful when accessing
     * the icon through the property system.
     */
    void setIcon(const QIcon &icon);

protected:
    virtual void keyPressEvent(QKeyEvent *event);

private:
    class ViewItemPrivate;
    ViewItemPrivate *d;

private:
    void drawReflection(QPainter *painter, const QRect &reflectionRect, const QIcon &icon);
    void drawReflection(QPainter *painter, const QRect &reflectionRect, const QPixmap &pm);

    QSize textRectSize() const;

};

} // namespace MediaCenter

#endif // VIEWITEM_H
