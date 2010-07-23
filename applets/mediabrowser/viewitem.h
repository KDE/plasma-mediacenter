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

namespace Plasma {
    class FrameSvg;
}

namespace Nepomuk {
    class Resource;
}

class KFileItem;
class QPixmap;

class ViewItem : public QGraphicsWidget
{
    Q_OBJECT

    Q_PROPERTY(QIcon icon READ icon WRITE setIcon);
    Q_PROPERTY(QString text READ text WRITE setText);
    Q_PROPERTY(int decorationSize READ decorationSize WRITE setDecorationSize);
    Q_PROPERTY(QModelIndex modelIndex READ index WRITE setModelIndex);
public:
    enum ItemType { LocalFileItem, RemoteFileItem };

    /**
     * Each ViewItem must be created passing the option object
     * describing its appearance constraints.
     */
    ViewItem(QGraphicsItem *parent = 0);
    ViewItem(const QStyleOptionViewItemV4 &option, QGraphicsItem *parent = 0);
    ~ViewItem();

    void setStyleOption(const QStyleOptionViewItemV4 &);
    QStyleOptionViewItemV4 styleOption() const;

    void setModelIndex(const QModelIndex &index);
    QModelIndex index() const;

    void setItemType(ItemType);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QSize itemSizeHint() const;

    void updateHoverRating(const QPoint &pos);

    void setRating(int rating);

    void setDrawBlurredText(bool set);
    bool drawBlurredText();

    bool isSelected();

    void showCornerIcons(bool set);
    void setIsNotFile(bool set);

    /**
     * This method allows setting a custom icon to be shown
     * in place of the one passed by the QStyleOptionGraphicsItem as
     * parameter of the paint function.
     * @note This is particularly useful when accessing
     * the icon through the property system.
     */
    void setIcon(const QIcon &icon);

    /**
     * @return the icon set with the setIcon function or a
     * null icon if none was set.
     */
    QIcon icon() const;

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
    void setSelected(bool set);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QStyleOptionViewItemV4 m_option;
    QModelIndex m_index;
    ItemType m_type;
    Plasma::FrameSvg *m_frameSvg;
    QPixmap m_preview;
    QPixmap m_selectIcon;
    QPixmap m_blurredText;
    QPixmap m_reflection;
    int m_hoverRating;
    int m_rating;
    Nepomuk::Resource *m_resource;
    bool m_nepomuk;
    bool m_blurred;
    bool m_selectByIcon;
    bool m_isSelected;
    bool m_isNotFile;
    QIcon m_icon;
    QString m_text;
    int m_decorationSize;

    friend class AbstractMediaItemView;

private:
    void drawReflection(QPainter *painter, const QRect &reflectionRect, const QIcon &icon);
    void drawReflection(QPainter *painter, const QRect &reflectionRect, const QPixmap &pm);
    void drawSelectIcon(QPainter *painter, const QStyleOptionGraphicsItem *option);
    void askForFilePreview();

    QRect ratingRect(const QRect &contentsRect) const;
    QRect selectRect(const QRect &contentsRect) const;
    QSize textRectSize() const;

private slots:
    void slotGotPreview(const KFileItem &item, const QPixmap &preview);

signals:
    void ratingActivated(int);
    void itemSelected();

};

#endif // VIEWITEM_H
