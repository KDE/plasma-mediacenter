#ifndef METADATAUPDATER_H
#define METADATAUPDATER_H

#include <QtCore/QThread>

#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/QueryServiceClient>
#include <nepomuk/term.h>

#include <QtCore/QMutex>
#include <QtCore/QAbstractItemModel>

namespace Nepomuk {
    namespace Query {
        class Term;
    }
    class Resource;
}

class MetadataUpdater : public QThread
{
    Q_OBJECT
public:
    explicit MetadataUpdater(const QList<int> &rolesRequested, QObject* parent = 0);
    virtual ~MetadataUpdater();
    void addResultToQueue(const QPersistentModelIndex &index, const Nepomuk::Query::Result& result);
    void quit();

signals:
    void gotMetadata(const QPersistentModelIndex &index, const QHash<int, QVariant> &values);

protected:
    virtual void run();

private:
    QList< int > m_rolesRequested;
    QList<QPersistentModelIndex> m_indices;
    QHash<QPersistentModelIndex, Nepomuk::Query::Result> m_results;
    bool m_shouldQuit;
    QMutex m_resultsMutex;
    QMutex m_quitMutex;

    QString mimetypeForResource(const Nepomuk::Resource& resource) const;
    QString urlForResource(const Nepomuk::Resource &resource) const;
    void fetchValuesForResult(const QPersistentModelIndex& index, const Nepomuk::Query::Result& result);
    bool areThereResultsToProcess();
    QPersistentModelIndex nextIndexToProcess();
    Nepomuk::Query::Result resultForIndex(const QPersistentModelIndex &index);
    bool shouldQuit();
};

#endif // METADATAUPDATER_H
