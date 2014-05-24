#ifndef SERIESMODEL_H
#define SERIESMODEL_H

#include <QtCore/QAbstractItemModel>
#include <mediacenter/pmcmedia.h>
#include "clusterer.h"

class SeriesModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit SeriesModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role) const;
    virtual int rowCount(const QModelIndex& parent) const;

private:
    Clusterer clusterer;
    QList <QSharedPointer<PmcMedia> > mediaData;
    QList <QString> clusters;
};

#endif // SERIESMODEL_H
