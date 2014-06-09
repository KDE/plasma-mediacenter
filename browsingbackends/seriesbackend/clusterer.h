#ifndef CLUSTERER_H
#define CLUSTERER_H
#include "cluster.h"
#include <mediacenter/pmcmedia.h>

class Clusterer: public QObject {
    Q_OBJECT
public:
    Clusterer();
    void buildCluster(QList<PmcMedia::Ptr> &mediaList);
    int size() const;

    QString label(int index) const;
    Cluster* cluster(int index) const;

signals:
    void sizeChanged(int size);

private:
    Cluster m_cluster;
    QList<Cluster*> m_clusters;

    Cluster* findAndInsert(Cluster &root, QStringList &labels);
    void makeListOfClusters();
    void mergeTree(Cluster *cluster);
    void populate(QList<PmcMedia::Ptr> &mediaList);
};

#endif // CLUSTERER_H
