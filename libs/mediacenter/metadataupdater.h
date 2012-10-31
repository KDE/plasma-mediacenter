/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
 *   Copyright 2011 Marco Martin <notmart@gmail.com>                       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef METADATAUPDATER_H
#define METADATAUPDATER_H

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

class MetadataUpdater : public QThread
{
    Q_OBJECT
public:
    explicit MetadataUpdater(const QList<int> &rolesRequested, QObject* parent = 0);
    virtual ~MetadataUpdater();
    void fetchMetadata(int row);
    void fetchMetadata(const QList<int> &rows);
    void setTerm(const Nepomuk2::Query::Term &term);

signals:
    void gotMetadata(int row, const QHash<int, QVariant> &values);
    void newResults(int count);
    void reset();

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
    bool areThereResultsToProcess();
    int nextIndexToProcess();
    Nepomuk2::Query::Result resultForRow(int row);
    bool hasTermChanged();
};

#endif // METADATAUPDATER_H
