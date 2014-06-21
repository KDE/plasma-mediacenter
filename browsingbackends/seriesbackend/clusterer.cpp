#include "clusterer.h"
#include <QUrl>
#include <QDebug>
#include <KDebug>

#include <cstdio>

Clusterer::Clusterer()
{
}

void Clusterer::buildCluster(QList< PmcMedia::Ptr >& mediaList)
{
    populate(mediaList);
    mergeTree(&m_cluster);
    makeListOfClusters();

    // qDebug() << "Video list size: " << mediaList.size();

    displayTree(&m_cluster);
}

Cluster* Clusterer::findAndInsert(Cluster& root, QStringList& labels)
{
    Cluster* node = &root;
    Q_FOREACH(QString label, labels) {
        node = node->insertChild(label);
    }
    return node;
}

/**
 * A, B (level1)
 * B -> {C, D} (C & D)
 *
 */
QString Clusterer::label(int index) const
{
    if (index > m_clusters.size()) {
        return QString("");
    }
    return m_clusters.at(index)->label();
}

Cluster* Clusterer::cluster(int index) const
{
    return m_clusters.at(index);
}

void Clusterer::makeListOfClusters()
{
    if (m_cluster.empty()) {
        return;
    }

    QList<Cluster*> queue;
    QList<Cluster*> tempClusters;
    queue.append(&m_cluster);
    while (!queue.isEmpty()) {
        QList<Cluster*> levelChildren;
        Q_FOREACH(Cluster * node, queue) {
            if (node->merged()) {
                tempClusters.append(node);
                qDebug() << "Number of merged nodes for: " << node->label() << " : " << node->mergedNodes().size();
                Q_FOREACH(Cluster * c, node->mergedNodes()) {
                    qDebug() << ">>>>>>>> " << c->label();
                }
            }
            levelChildren.append(node->children());
        }
        queue = levelChildren;
    }

    kDebug() << "TEMP CLUSTERS";
    Q_FOREACH(Cluster * c, tempClusters) {
        kDebug() << c->label();
    }

    Cluster* tempClusterHolder = new Cluster();
    tempClusterHolder->children().append(tempClusters);
    mergeNodesAtSameLevel(tempClusterHolder);
    m_clusters = tempClusterHolder->children();

    kDebug() << "MMMM CLUSTERS";
    Q_FOREACH(Cluster * c, m_clusters) {
        kDebug() << c->label();
    }

    emit sizeChanged(m_clusters.size());
}

void Clusterer::displayTree(Cluster* node)
{
    for (int i = 0; i < node->level(); i++) {
        printf(" ");
    }
    printf("%s", node->label().toLocal8Bit().data());
    if (node->merged()) {
        printf("|");
        Q_FOREACH(auto mergedNode, node->mergedNodes()) {
            printf("%s(%u) ", mergedNode->label().toLocal8Bit().data(), mergedNode);
        }
    }
    printf("\n");
    Q_FOREACH(auto child, node->children()) {
        displayTree(child);
    }
    fflush(stdout);
}

/**
 * Root -> {A, C, E}
 * tryMerge(A, B) {remove B if merged}
 * tryMerge(A, C)
 */
void Clusterer::mergeTree(Cluster* cluster)
{

    Q_FOREACH(auto c, cluster->children()) {
//         kDebug() << "Trying to merge children of " << c << c->label();
        mergeTree(c);
    }
//     kDebug() << "Starting with " << cluster << cluster->label();

    mergeNodesAtSameLevel(cluster);
}

void Clusterer::mergeNodesAtSameLevel(Cluster* cluster)
{
    int noOfChildren = cluster->children().size();
    int i = 0;
    while (i < noOfChildren - 1) {
        auto c1 = cluster->children().at(i); // A
//         kDebug() << "Beginning with " << c1 << c1->label();
        int j = i + 1; // B
        while (j < noOfChildren) {
            auto c2 = cluster->children().at(j); // B (j)
//             kDebug() << "Picking child " << c2 << c2->label();
//             kDebug() << c1->fuzzyString() << c2->fuzzyString();
//             kDebug() << "COSINE Similarity " << Cluster::cosineSimilarity(c1->fuzzyString(), c2->fuzzyString());
            bool merged = c1->mergeNode(c2); // merge A & B
            if (merged) {
//                 kDebug() << "Merged " << cluster->children().at(j) << cluster->children().at(j)->label() << " with " << c1 << c1->label();
                Q_FOREACH(Cluster * c2Child, c2->children()) {
                    if (c2Child->merged()) {
                        c1->children().append(c2Child);
                    }
                }
                cluster->children().removeAt(j);
                j--; // A, C
                noOfChildren--;
            }
            j++;
        }
        i++;
    }
}

void Clusterer::populate(QList< PmcMedia::Ptr >& mediaList)
{
    if (m_cluster.empty()) {
        m_cluster = Cluster("root");
    }
    Q_FOREACH(PmcMedia::Ptr media, mediaList) {
        QUrl url(media->url());
        QString fullLabel = url.toLocalFile();
        QStringList labels = fullLabel.split("/");
        if (labels.isEmpty()) {
            continue;
        }
        if (labels.first() == "") {
            labels.removeFirst();
        }
        auto node = findAndInsert(m_cluster, labels);
        node->setUrl(fullLabel);
    }
}

int Clusterer::size() const
{
    return m_clusters.size();
}
