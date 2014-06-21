#include "clustertest.h"

#include <qtest_kde.h>

QTEST_KDEMAIN(ClusterTest, NoGUI);

void ClusterTest::initTestCase()
{
    // Called before the first testfunction is executed
}

void ClusterTest::cleanupTestCase()
{
    // Called after the last testfunction was executed
}

void ClusterTest::init()
{
    // Called before each testfunction is executed
}

void ClusterTest::cleanup()
{
    // Called after every testfunction
}

void ClusterTest::testCosineSimilarity()
{
    QString string1 = "abcde";
    QStringList fs1 = string1.split("");

    QString string2 = "bcdef";
    QStringList fs2 = string2.split("");

    QVERIFY(Cluster::cosineSimilarity(fs1, fs2) - 0.888889 < 0.0001);
}

void ClusterTest::shouldFuzzifyLabel()
{
    Cluster c("Movie[1234]Media.FileFormat[Lang]-Credits.avi");

    QStringList expectedLabels;
    expectedLabels << "Movie" << "1234" << "Media" << "FileFormat" << "Lang" << "Credits" << "avi";
    QCOMPARE(c.fuzzyString(),expectedLabels);
}

#include "clustertest.moc"
