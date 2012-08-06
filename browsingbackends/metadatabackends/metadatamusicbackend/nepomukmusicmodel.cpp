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


NepomukMusicModel::NepomukMusicModel(QObject* parent): QAbstractListModel(parent)
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

void NepomukMusicModel::setTerm(Nepomuk::Query::Term term, const QString &iconName)
{
    m_icon = iconName;
    m_term = term;
    updateModel();
}

void NepomukMusicModel::updateModel()
{
    Nepomuk::Query::Query myQuery;
    Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);

    connect(queryClient, SIGNAL(newEntries(QList<Nepomuk::Query::Result>)),
            this, SLOT(newEntries(QList<Nepomuk::Query::Result>)));
    connect(queryClient, SIGNAL(entriesRemoved(QList<QUrl>)),SLOT(entriesRemoved(QList<QUrl>)));
    connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));

    myQuery.setTerm(m_term);
    kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();

    m_queryResults.clear();
    reset();

    queryClient->query(myQuery);
}

void NepomukMusicModel::setResourceType(Nepomuk::Types::Property property, const QString &iconName)
{
    Nepomuk::Query::ComparisonTerm ct(property, Nepomuk::Query::Term());
    ct.setInverted(true);
    m_term = ct;
    m_icon = iconName;
    updateModel();
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
    case MediaCenter::ResourceIdRole:
        return m_queryResults.at(index.row()).resource().resourceUri();
    case MediaCenter::MediaTypeRole:
        return "audio";
    case MediaCenter::MediaUrlRole:
        return m_queryResults.at(index.row()).resource().property(Nepomuk::Vocabulary::NIE::url()).toString();
    }

    return QVariant();
}

int NepomukMusicModel::rowCount(const QModelIndex& parent) const
{
    return m_queryResults.size();
}

void NepomukMusicModel::newEntries(const QList< Nepomuk::Query::Result >& entries)
{
    Q_FOREACH (const Nepomuk::Query::Result &res, entries) {
        m_queryResults.append(res);
    }
}

void NepomukMusicModel::entriesRemoved(QList< QUrl > entries)
{

}

void NepomukMusicModel::finishedListing()
{
    reset();
}

void NepomukMusicModel::error(const QString &message)
{
    kDebug() << message;
}