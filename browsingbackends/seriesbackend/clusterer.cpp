#include "clusterer.h"
#include <QUrl>
#include <QDebug>

Clusterer::Clusterer() {
}

void Clusterer::buildCluster(QList< PmcMedia::Ptr >& mediaList) {
    populate(mediaList);
    mergeTree(&m_cluster);
    makeListOfClusters();
    qDebug() << "Video list size: " << mediaList.size();
}

Cluster* Clusterer::findAndInsert(Cluster& root, QStringList& labels) {
    Cluster *node = &root;
    Q_FOREACH(QString label, labels) {
        node = node->insertChild(label);
    }
    return node;
}

QString Clusterer::label(int index) const {
    if(index > m_clusters.size()) {
        return QString("");
    }
    return m_clusters.at(index)->label();
}

Cluster* Clusterer::cluster(int index) const
{
    return m_clusters.at(index);
}

void Clusterer::makeListOfClusters() {
    if(m_cluster.empty()) {
        return;
    }
    QList<Cluster*> queue;
    queue.append(&m_cluster);
    while(!queue.isEmpty()) {
        QList<Cluster*> levelChildren;
        Q_FOREACH(auto node, queue) {
            if(node->merged()) {
                m_clusters.append(node);
                qDebug() << "Number of children for: " << node->label() << " : " << node->children().size();
            }
            levelChildren.append(node->children());
        }
        queue.clear();
        queue = levelChildren;
    }
    emit sizeChanged(m_clusters.size());
}


void Clusterer::mergeTree(Cluster* cluster) {
    Q_FOREACH(auto c, cluster->children()) {
        mergeTree(c);
    }
    int noOfChildren = cluster->children().size();
    int i = 0;
    while(i < noOfChildren - 1) {
        auto c1 = cluster->children().at(i);
        int j = i + 1;
        while(j < noOfChildren) {
            auto c2 = cluster->children().at(j);
            bool merged = c1->mergeNode(c2);
            if(merged) {
                j--;
                noOfChildren -= 1;
                cluster->children().removeAt(j);
            }
            j++;
        }
        i++;
    }
}

void Clusterer::populate(QList< PmcMedia::Ptr >& mediaList) {
    if(m_cluster.empty()) {
        m_cluster = Cluster("root");
    }
    Q_FOREACH(PmcMedia::Ptr media, mediaList) {
        QUrl url(media->url());
        QString fullLabel = url.toLocalFile();
        QStringList labels = fullLabel.split("/");
        if(labels.isEmpty()) {
            continue;
        }
        if(labels.first() == "") {
            labels.removeFirst();
        }
        auto node = findAndInsert(m_cluster, labels);
        node->setUrl(fullLabel);
    }
}

int Clusterer::size() const {
    return m_clusters.size();
}
