#include "clusterer.h"
#include <QUrl>
#include <QQueue>

#include <KDebug>

Clusterer::~Clusterer()
{
    if (m_rootCluster) {
        delete m_rootCluster;
    }
}

void Clusterer::buildCluster(QList< PmcMedia::Ptr >& mediaList)
{
    m_rootCluster = populate(mediaList);
    mergeTree(m_rootCluster);
    makeListOfClusters(m_rootCluster);

    displayTree(m_rootCluster);
}

Cluster* Clusterer::findAndInsert(Cluster* root, QStringList& labels)
{
    Cluster* node = root;
    Q_FOREACH(QString label, labels) {
        node = node->insertChild(label);
    }
    return node;
}

QString Clusterer::label(int index) const
{
    if (index > m_clusters.size()) {
        return QString();
    }
    return m_clusters.at(index)->label();
}

Cluster* Clusterer::cluster(int index) const
{
    return m_clusters.at(index);
}

void Clusterer::makeListOfClusters(Cluster *rootCluster)
{
    if (!m_rootCluster || m_rootCluster->empty()) {
        return;
    }

    QQueue<Cluster*> queue;
    QList<Cluster*> tempClusters;

    queue.enqueue(rootCluster);
    while (!queue.isEmpty()) {
        QQueue<Cluster*> childrenAtThisLevel;

        Q_FOREACH(Cluster* node, queue) {
            if (node->merged()) {
                tempClusters.append(node);
            }
            childrenAtThisLevel.append(node->children());
        }
        queue = childrenAtThisLevel;
    }

    auto tempClusterHolder = new Cluster("root");
    tempClusterHolder->children().append(tempClusters);
    mergeNodeWithSiblings(tempClusterHolder);
    m_clusters = tempClusterHolder->children();

    emit sizeChanged(m_clusters.size());
}

void Clusterer::displayTree(Cluster* node)
{
    QStringList spaces; for(int i=0;i<node->level();i++) spaces << " ";

    QString output = QString("%1%2").arg(spaces.join(" ")).arg(node->label());

    if (node->merged()) {
        output.append("|");
        Q_FOREACH(Cluster* mergedNode, node->mergedNodes()) {
            output.append(QString("%1(%2)").arg(mergedNode->label()).arg(quintptr(mergedNode)));
        }
    }

    kDebug() << output;

    Q_FOREACH(auto child, node->children()) {
        displayTree(child);
    }
}

/**
 * Root -> {A, C, E}
 * tryMerge(A, B) {remove B if merged}
 * tryMerge(A, C)
 */
void Clusterer::mergeTree(Cluster* clusterRoot)
{
    Q_FOREACH(auto c, clusterRoot->children()) {
        mergeTree(c);
    }

    mergeNodeWithSiblings(clusterRoot);
}

void Clusterer::mergeNodeWithSiblings(Cluster* cluster)
{
    int numberOfChildren = cluster->children().size();

    for (int i = 0; i < numberOfChildren - 1; i++) {
        auto pivotChild = cluster->children().at(i);

        for (int j = i + 1; j < numberOfChildren; j++) {
            auto sibling = cluster->children().at(j);
            bool mergeSuccessful = pivotChild->mergeNode(sibling);

            if (mergeSuccessful) {
                Q_FOREACH(Cluster *siblingChild, sibling->children()) {
                    if (siblingChild->merged()) {
                        pivotChild->children().append(siblingChild);
                    }
                }

                cluster->children().removeAt(j--);
                numberOfChildren--;
            }
        }
    }
}

Cluster* Clusterer::populate(QList< PmcMedia::Ptr >& mediaList)
{
    auto rootCluster = new Cluster("root");

    Q_FOREACH(PmcMedia::Ptr media, mediaList) {
        QUrl url = media->url();
        QString fullLabel = url.toLocalFile();
        QStringList pathLabels = fullLabel.split("/");

        if (pathLabels.isEmpty()) {
            continue;
        }
        if (pathLabels.first().isEmpty()) {
            pathLabels.removeFirst();
        }

        Cluster* node = findAndInsert(rootCluster, pathLabels);
        node->setUrl(fullLabel);
    }

    return rootCluster;
}

int Clusterer::clusterSize() const
{
    return m_clusters.size();
}
