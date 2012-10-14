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
