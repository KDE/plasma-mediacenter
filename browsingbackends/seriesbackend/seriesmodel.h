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
    bool setCurrentCluster(int row);

private:
    int currentCluster = -1;
    Clusterer clusterer;
    QList <QSharedPointer<PmcMedia> > mediaData;
    QList <QString> clusters;
private slots:
    void clusterSizeUpdated(int);
};

#endif // SERIESMODEL_H
