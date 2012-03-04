#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <../../home/sinny/kde/src/qt/include/QtCore/qabstractitemmodel.h>
#include <qlocale.h>

class Category
{
public:
    Category(QString label, QString icon);
    QString label() const;
    QString icon() const;
private:
    QString m_icon;
    QString m_label;
};

class CategoriesModel : public QAbstractItemModel
{

public:
    explicit CategoriesModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

private:
    QList<Category> m_categories;
};

#endif // CATEGORIESMODEL_H
