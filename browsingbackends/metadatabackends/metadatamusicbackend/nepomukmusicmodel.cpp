/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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

#include "nepomukmusicmodel.h"
#include <Nepomuk/File>
#include <Nepomuk/Query/AndTerm>
#include <Nepomuk/Query/NegationTerm>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Tag>
#include <Nepomuk/Variant>
#include <nepomuk/comparisonterm.h>
#include <nepomuk/literalterm.h>
#include <nepomuk/queryparser.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/standardqueries.h>
#include <Nepomuk/Vocabulary/NMM>
#include <nepomuk/nfo.h>
#include <nepomuk/nie.h>
#include <KDebug>
#include <mediacenter/mediacenter.h>


NepomukMusicModel::NepomukMusicModel(QObject* parent): QAbstractItemModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

void NepomukMusicModel::setTerm(Nepomuk::Types::Property term, const QString &iconName)
{
    m_icon = iconName;

    Nepomuk::Query::Query myQuery;
    Nepomuk::Query::ComparisonTerm ct(term, Nepomuk::Query::Term());
    ct.setInverted(true);
    Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);
    connect(queryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
            this, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
    connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));
    myQuery.setTerm(ct);
    kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();
    m_queryResults.clear();
    reset();
    queryClient->query(myQuery);
}

QVariant NepomukMusicModel::data(const QModelIndex& index, int role) const
{

    switch(role) {
    case Qt::DecorationRole:
        return m_icon;
    case Qt::DisplayRole:
        return m_queryResults.at(index.row()).resource().genericLabel();
    case MediaCenter::IsExpandableRole:
        return true;
    }

    return QVariant();
}

int NepomukMusicModel::columnCount(const QModelIndex& parent) const
{

    return 1;
}

int NepomukMusicModel::rowCount(const QModelIndex& parent) const
{

    return m_queryResults.size();
}

QModelIndex NepomukMusicModel::parent(const QModelIndex& child) const
{

    return QModelIndex();
}

QModelIndex NepomukMusicModel::index(int row, int column, const QModelIndex& parent) const
{

    return createIndex(row,column);
}

void NepomukMusicModel::newEntries(const QList< Nepomuk::Query::Result >& entries)
{
      Q_FOREACH (Nepomuk::Query::Result res, entries) {
        m_queryResults.append(res);
    }
}

void NepomukMusicModel::finishedListing()
{
    reset();
}
