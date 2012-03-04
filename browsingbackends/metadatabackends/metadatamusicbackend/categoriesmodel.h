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
