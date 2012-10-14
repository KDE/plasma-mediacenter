/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Shantanu Tushar <shantanu@kde.org>                     *
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

#ifndef PMCMETADATAMODEL_H
#define PMCMETADATAMODEL_H

#include <QtCore/QAbstractItemModel>

#include <Nepomuk/Query/Result>
#include <Nepomuk/Query/Term>
#include <Nepomuk/Types/Property>
#include <nepomuk/resource.h>
#include <nepomuk/comparisonterm.h>

#include "mediacenter_export.h"
#include "mediacenter.h"

namespace Nepomuk {
namespace Query {
class ResourceTypeTerm;
}
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
    void showMediaForProperty(Nepomuk::Types::Property property);
    void setTerm(const Nepomuk::Query::Term &term);
    void addFilter(const Nepomuk::Types::Property& property, const Nepomuk::Query::Term& term, Nepomuk::Query::ComparisonTerm::Comparator comparator = Nepomuk::Query::ComparisonTerm::Contains);

public Q_SLOTS:
    void clearAllFilters();
    void saveMetadata(const QPersistentModelIndex &persistentIndex, const QHash< int, QVariant >& values);

protected Q_SLOTS:
    void finishedListing();
    void updateModel();
    void error(const QString &message);
    void delayedPreview();

protected:
    QString fetchPreview(const KUrl& url, const QModelIndex& index);
    QString urlForResource(const Nepomuk::Resource &resource) const;
    QString mimetypeForResource(const Nepomuk::Resource &resource) const;
    inline QList<int> rolesNeeded() const;
    bool fetchMetadataIfRequired(const QModelIndex &index);

private Q_SLOTS:
    void newEntries(const QList<Nepomuk::Query::Result> &results);
    void showPreview(const KFileItem &item, const QPixmap &preview);
    void previewFailed(const KFileItem &item);

private:
    class Private;
    Private * const d;
};

#endif // PMCMETADATAMODEL_H
