#include "cluster.h"

#include <QtCore/qmath.h>

#define COS_SIM_THRESHOLD 0.4

Cluster::Cluster(QString label, int level)
    : m_label(label)
    , m_level(level)
{
    fuzzifyLabel();
}

QList< Cluster* >& Cluster::children()
{
    return m_children;
}

bool Cluster::empty()
{
    return m_empty;
}


double Cluster::cosineSimilarity(QStringList fs1, QStringList fs2)
{
    auto charCount1 = charCount(fs1);
    auto charCount2 = charCount(fs2);

    double numerator = 0;
    Q_FOREACH(auto key, charCount1.keys()) {
        if (charCount2.contains(key)) {
            numerator += (charCount1[key] * charCount2[key]);
        }
    }
    double sumCounts1 = 0, sumCounts2 = 0;
    Q_FOREACH(auto value, charCount1.values()) {
        sumCounts1 += (value * value);
    }
    Q_FOREACH(auto value, charCount2.values()) {
        sumCounts2 += (value * value);
    }
    double denominator = sqrt(sumCounts1) * sqrt(sumCounts2);

    return denominator == 0 ? 0 : (numerator / denominator);
}

QHash<QString, int> Cluster::charCount(QStringList word)
{
    QHash<QString, int> charCounts;

    Q_FOREACH(QString ch, word) {
        if (!charCounts.contains(ch)) {
            charCounts[ch] = 0;
        }
        charCounts[ch]++;
    }

    return charCounts;
}

// Folder -> blah1, blah2
// file3, file4
//
// blah1, blah2
// file3, file4
//
// cluster(blah1), cluster(file3)

void Cluster::fuzzifyLabel()
{
    m_fuzzyString = m_label.split(QRegExp("[^A-Za-z0-9]+"), QString::SkipEmptyParts);
}

QStringList Cluster::fuzzyString()
{
    return this->m_fuzzyString;
}


Cluster* Cluster::insertChild(QString& label)
{
    Q_FOREACH(Cluster* child, m_children) {
        if (child->label() == label) {
            return child;
        }
    }

    auto child = new Cluster(label, m_level + 1);
    m_children.append(child);

    return child;
}

QString Cluster::label() const
{
    return m_label;
}

int Cluster::level()
{
    return m_level;
}

bool Cluster::merged()
{
    return m_merged;
}

QList< Cluster* >& Cluster::mergedNodes()
{
    return m_mergedNodes;
}

bool Cluster::mergeCompatible(Cluster* cluster)
{
    double cosSimilarity = cosineSimilarity(m_fuzzyString, cluster->m_fuzzyString);

    return cosSimilarity >= COS_SIM_THRESHOLD;
}

/**
 * A -> C, D
 * B (this)
 * if(A is mergeable with B) {B->merged_nodes = (A, C, D), A->merged_nodes = {}}
 */
bool Cluster::mergeNode(Cluster* cluster)
{
    if (cluster == this) {
        return false;
    }

    if (mergeCompatible(cluster)) {
        if (cluster->merged()) {
            Q_FOREACH(auto mergedNode, cluster->mergedNodes()) {
                if (!m_mergedNodes.contains(mergedNode)) {
                    m_mergedNodes.append(mergedNode);
                }
            }
            cluster->setMerged(false);
        }
        m_mergedNodes.append(cluster);
        setMerged(true);
        resetLabels();
        return true;
    }

    return false;
}

void Cluster::resetLabels()
{
    //TODO: Choose better label/fuzzy string to have
}


void Cluster::setMerged(bool merged)
{
    m_merged = merged;
    if (!merged) {
        m_mergedNodes.clear();
    }
}

void Cluster::setUrl(const QString& url)
{
    m_url = url;
}

QString Cluster::url() const
{
    return m_url;
}
