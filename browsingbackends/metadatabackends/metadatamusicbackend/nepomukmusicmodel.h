#ifndef NEPOMUKMUSICMODEL_H
#define NEPOMUKMUSICMODEL_H

#include <QAbstractItemModel>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Types/Property>

class CategoryData
{
public:
    CategoryData(QString icon, Nepomuk::Types::Property property);
    Nepomuk::Types::Property property() const;
    QString icon() const;
private:
    Nepomuk::Types::Property m_property;
    QString m_icon;
};

class NepomukMusicModel : public QAbstractItemModel
{

    Q_OBJECT
    protected Q_SLOTS:
    void newEntries(const QList< Nepomuk::Query::Result > &entries);
    void finishedListing();
public:
    explicit NepomukMusicModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    void setTerm(Nepomuk::Types::Property term);

private:
    QList< Nepomuk::Query::Result > m_queryResults;
    QList<CategoryData> m_categoryData;
};

#endif // NEPOMUKMUSICMODEL_H
