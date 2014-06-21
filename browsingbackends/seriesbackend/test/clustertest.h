#ifndef CLUSTERTEST_H
#define CLUSTERTEST_H

#include "../cluster.h"

#include <QObject>

class ClusterTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testCosineSimilarity();
    void shouldFuzzifyLabel();
};

#endif // CLUSTERTEST_H
