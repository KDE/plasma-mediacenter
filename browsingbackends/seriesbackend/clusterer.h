#ifndef CLUSTERER_H
#define CLUSTERER_H

#include "cluster.h"

#include <mediacenter/pmcmedia.h>

class Clusterer: public QObject
{
    Q_OBJECT
public:
    virtual ~Clusterer();

    void buildCluster(QList<PmcMedia::Ptr>& mediaList);
    int clusterSize() const;

    QString label(int index) const;
    Cluster* cluster(int index) const;

signals:
    void sizeChanged(int size);

private:
    using QObject::QObject;
    Cluster *m_rootCluster = nullptr;
    QList<Cluster*> m_clusters;

    Cluster* findAndInsert(Cluster* root, QStringList& labels);
    void makeListOfClusters(Cluster *rootCluster);
    void mergeNodeWithSiblings(Cluster* cluster);
    void mergeTree(Cluster* clusterRoot);
    Cluster* populate(QList< PmcMedia::Ptr >& mediaList);
    void displayTree(Cluster* node);
};

#endif // CLUSTERER_H
