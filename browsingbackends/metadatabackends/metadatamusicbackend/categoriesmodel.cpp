#include "categoriesmodel.h"

#include <mediacenter/mediacenter.h>

CategoriesModel::CategoriesModel(QObject* parent): QAbstractItemModel(parent)
{
    m_categories.append(Category("All Songs", "audio", Category::AllMusic));
    m_categories.append(Category("Artists", "view-media-artist", Category::Artists));
    m_categories.append(Category("Albums", "tools-media-optical-copy", Category::Albums));

    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

QVariant CategoriesModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
        case Qt::DecorationRole:{
            return m_categories.at(index.row()).icon();
        }
        case Qt::DisplayRole:{
            return m_categories.at(index.row()).label();
        }
        case MediaCenter::IsExpandableRole:
            return true;
    }

    return QVariant();
}

int CategoriesModel::columnCount(const QModelIndex& parent) const
{

  return 1;
}

int CategoriesModel::rowCount(const QModelIndex& parent) const
{

    return m_categories.size();
}

QModelIndex CategoriesModel::parent(const QModelIndex& child) const
{

    return QModelIndex();
}

QModelIndex CategoriesModel::index(int row, int column, const QModelIndex& parent) const
{

    return createIndex(row,column);
}


Category::Category(QString label, QString icon, CategoryType categoryType)
    : m_icon(icon),
    m_label(label),
    m_categoryType(categoryType)
{
}

QString Category::icon() const
{
    return m_icon;
}

QString Category::label() const
{
    return m_label;
}

Category::CategoryType Category::categoryType() const
{
    return m_categoryType;
}

Category::CategoryType CategoriesModel::categoryTypeForIndex(int index) const
{
    if (index >= m_categories.size())
        return Category::AllMusic;

    return m_categories.at(index).categoryType();
}
