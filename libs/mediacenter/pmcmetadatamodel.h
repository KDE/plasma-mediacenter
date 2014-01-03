/***********************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                                *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
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

#ifndef PMCMETADATAMODEL_H
#define PMCMETADATAMODEL_H

#include <QtCore/QAbstractItemModel>

#include <Nepomuk2/Query/Result>
#include <Nepomuk2/Query/Term>
#include <Nepomuk2/Types/Property>
#include <Nepomuk2/Resource>
#include <Nepomuk2/Query/ComparisonTerm>

#include "mediacenter_export.h"
#include "mediacenter.h"

namespace Nepomuk2 {
namespace Query {
class ResourceTypeTerm;
}
}

namespace odb {
    class database;
}

class QPixmap;
class KFileItem;

class MEDIACENTER_EXPORT PmcMetadataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PmcMetadataModel(QObject* parent = 0);
    virtual ~PmcMetadataModel();
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    void showMediaType(MediaCenter::MediaType mediaType);
    void showMediaForProperty(Nepomuk2::Types::Property property);
    void setTerm(const Nepomuk2::Query::Term &term);
    void addTerm(const Nepomuk2::Query::Term &term);
    void addFilter(const Nepomuk2::Types::Property& property, const Nepomuk2::Query::Term& term, Nepomuk2::Query::ComparisonTerm::Comparator comparator = Nepomuk2::Query::ComparisonTerm::Contains);
    void setSearchTerm(const QString &searchTerm);
    void setDefaultDecoration(const QVariant &decoration);

public Q_SLOTS:
    void clearAllFilters();
    void saveMetadata(int row, const QHash< int, QVariant >& values);

signals:
    void queryStarted();
    void queryFinished();
    void queryError(const QString &message);

protected Q_SLOTS:
    void finishedListing();
    void updateModel();
    void delayedPreview();

protected:
    QString fetchPreview(const KUrl& url, const QModelIndex& index);
    QString urlForResource(const Nepomuk2::Resource &resource) const;
    QString mimetypeForResource(const Nepomuk2::Resource &resource) const;
    inline QList<int> rolesNeeded() const;
    void resetModel();

private Q_SLOTS:
    void newEntries(int count);
    void showPreview(const KFileItem &item, const QPixmap &preview);
    void previewFailed(const KFileItem &item);
    void fetchMetadata();
    void handleUpdaterReset();
    void signalUpdate(const QPersistentModelIndex &index, const QString &displayString = QString());

private:
    class Private;
    Private * const d;

    QVariant decorationForMetadata(const QVariant& metadataValue, const QModelIndex& index) const;
    QVariant metadataValueForRole(const QModelIndex& index, int role) const;
    void saveMetadataToDb(QHash< int, QVariant > values);
    odb::database* initDb();
    void createDbSchema(odb::database* db);
};

#endif // PMCMETADATAMODEL_H
