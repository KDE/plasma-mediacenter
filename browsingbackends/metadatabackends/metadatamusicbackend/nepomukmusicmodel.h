#ifndef NEPOMUKMUSICMODEL_H
#define NEPOMUKMUSICMODEL_H

#include <QAbstractItemModel>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Types/Property>

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
    void setTerm(Nepomuk::Types::Property term, const QString& iconName);

private:
    QString m_icon;
    QList< Nepomuk::Query::Result > m_queryResults;
};

#endif // NEPOMUKMUSICMODEL_H
