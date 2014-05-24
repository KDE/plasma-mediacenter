#ifndef CLUSTER_H
#define CLUSTER_H
#include <boost/concept_check.hpp>
#include <QHash>
#include <QStringList>

class Cluster {
public:
    Cluster();
    Cluster(QString label, int level=0);

    QList<Cluster*> children();
    bool empty();
    Cluster* insertChild(QString &label);
    QString label() const;
    int level();
    bool merged();
    bool mergeNode(Cluster *cluster);
    QList<Cluster*> mergedNodes();
    QStringList fuzzyString();

    void setMerged(bool merged);

    static double cosineSimilarity(QStringList& fs1, QStringList& fs2);

private:
    bool m_empty = false;
    QStringList m_fuzzyString;
    QString m_label;
    int m_level;
    QList<Cluster*> m_children;
    bool m_merged;
    QList<Cluster*> m_mergedNodes;

    void fuzzifyLabel();
    bool mergeCompatible(Cluster *cluster);
    void resetLabels();

    static QHash< QString, int > charCount(QStringList word);
};

#endif // CLUSTER_H
