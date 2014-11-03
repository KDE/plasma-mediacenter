/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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

#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QAbstractItemModel>
#include <QLocale>

class Category
{
public:
    enum CategoryType { AllMusic, Artists, Albums };

    Category(QString label, QString icon, CategoryType categoryType);
    QString label() const;
    QString icon() const;
    CategoryType categoryType() const;

private:
    QString m_icon;
    QString m_label;
    CategoryType m_categoryType;
};

class CategoriesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CategoriesModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

    Category::CategoryType categoryTypeForIndex(int index) const;

private:
    QList<Category> m_categories;
};

#endif // CATEGORIESMODEL_H
