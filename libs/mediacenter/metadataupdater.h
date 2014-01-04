/***********************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>          `                     *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
 *   Copyright 2011 Marco Martin <notmart@gmail.com>                               *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef METADATAUPDATER_H
#define METADATAUPDATER_H

#include "mediacenter_export.h"
#include <QtCore/QThread>

#include <Nepomuk2/Query/Result>
#include <Nepomuk2/Query/QueryServiceClient>
#include <Nepomuk2/Query/Term>

#include <QtCore/QMutex>
#include <QtCore/QWaitCondition>
#include <QtCore/QAbstractItemModel>

namespace Nepomuk2 {
    namespace Query {
        class Term;
    }
    class Resource;
}

class MEDIACENTER_EXPORT MetadataUpdater : public QThread
{
    Q_OBJECT
public:
    explicit MetadataUpdater(QObject* parent = 0);
    virtual ~MetadataUpdater();
    void fetchMetadata(int row);
    void fetchMetadata(const QList<int> &rows);
    void setTerm(const Nepomuk2::Query::Term &term);

signals:
    void gotMetadata(int row, const QHash<int, QVariant> &values);
    void newResults(int count);
    void reset();
    void queryStarted();
    void queryFinished();
    void queryError(const QString &message);

protected:
    virtual void run();

protected slots:
    void runQuery();
    void processPendingIndices();

private slots:
    void newEntries(const QList<Nepomuk2::Query::Result> &results);
    void finishedListing();

private:
    QList<int> m_rolesRequested;
    QList<int> m_indices;
    bool m_termChanged;
    Nepomuk2::Query::Term m_term;
    QMutex m_termMutex;
    QMutex m_resultsMutex;
    QMutex m_indicesMutex;
    QList<Nepomuk2::Query::Result> m_resultList;
    Nepomuk2::Query::QueryServiceClient *m_queryServiceClient;

    QString mimetypeForResource(const Nepomuk2::Resource& resource) const;
    QString urlForResource(const Nepomuk2::Resource &resource) const;
    void fetchValuesForResult(int i, const Nepomuk2::Query::Result& result);
    bool areThereIndicesToProcess();
    int nextIndexToProcess();
    Nepomuk2::Query::Result resultForRow(int row);
    bool hasTermChanged();
};

#endif // METADATAUPDATER_H
