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


#include "metadatamusicmodel.h"
#include <mediacenter/mediacenter.h>
#include <mediacenter/abstractbrowsingbackend.h>

#include <QtGui/QIcon>
#include <QEvent>
#include <QDeclarativePropertyMap>
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

#include "kdebug.h"

#include <nepomuk/nfo.h>
#include <nepomuk/nie.h>

MetadataMusicModel::Category::Category ( const QString& label, const QString& icon )
{
    m_label = label;
    m_icon = icon;
}

QString MetadataMusicModel::Category::icon() const
{
    return m_icon;
}

QString MetadataMusicModel::Category::label() const
{
    return m_label;
}

MetadataMusicModel::MetadataMusicModel (QObject* parent)
    : AbstractMetadataModel (parent)
{
//     if (metadataModel()) {
//         metadataModel()->setProperty("resourceType", "nfo:Audio");
//         metadataModel()->setProperty("mimeType", "");
//         metadataModel()->setProperty("limit", 500);
//            
//     }
//     else {
//         kDebug() << "WARNING: Constructor called before metadataModel set :/";
//     }

    initializeCategories();
}

void MetadataMusicModel::initializeCategories()
{
    m_categories << Category("All Songs", "audio") << Category("Artist", "view-media-artist") << Category("Albums", "tools-media-optical-copy");
    m_showingCategories = true;
}

MetadataMusicModel::~MetadataMusicModel()
{

}

QVariant MetadataMusicModel::data (const QModelIndex& index, int role) const
{
    kDebug() << "ASKING " << index << " ROLE " << role;
    if (!metadataModel() || index.row() >= rowCount() || index.row() < 0) {
        return QVariant();
    }

    switch (role) {
        case MediaCenter::MediaTypeRole:
            return "audio";
            break;
        case Qt::DecorationRole:
            if (m_showingCategories) {
                return m_categories.at(index.row()).icon();
            } 
            break;
        case Qt::DisplayRole:
            if(m_showingCategories) {
                return m_categories.at(index.row()).label();
            } else if(m_usingNepomukDirectly) {
                return m_queryResults.at(index.row()).resource().genericLabel();
            } else {
                return AbstractMetadataModel::data(index, role);
            }
            break;
        case MediaCenter::IsExpandableRole:
            if (m_showingCategories) {
                return true;
            } else if (m_usingNepomukDirectly) {
                return false;
            }
            break;
        default:
            if (!m_showingCategories && !m_usingNepomukDirectly) {
                return AbstractMetadataModel::data(index, role);
            }
    }

    return QVariant();
}

int MetadataMusicModel::rowCount(const QModelIndex& parent) const
{
    if(m_showingCategories) {
        return m_categories.count();
    } else if(m_usingNepomukDirectly){
        return m_queryResults.count();
    }
    return AbstractMetadataModel::rowCount(parent);
}

void MetadataMusicModel::finishedListing()
{
    reset();
}

void MetadataMusicModel::newEntries(const QList< Nepomuk::Query::Result >& entries)
{
    Q_FOREACH (Nepomuk::Query::Result res, entries) {
        m_queryResults.append(res);
    }
}

void MetadataMusicModel::error(const QString &e)
{
    kDebug() << e;
}

QModelIndex MetadataMusicModel::index(int row, int column, const QModelIndex& parent) const
{
    if(m_usingNepomukDirectly || m_showingCategories)
        return createIndex(row, column);
    else {
        return AbstractMetadataModel::index(row,column,parent);
    }
}

bool MetadataMusicModel::browseTo(int row)
{
    if (m_showingCategories) {
        m_showingCategories = false;
        switch(row) {
            case 0:
                m_usingNepomukDirectly = false;
                metadataModel()->setProperty("resourceType", "nfo:Audio");
                metadataModel()->setProperty("mimeType", "");
                metadataModel()->setProperty("limit", 500);
                break;
            case 1: {
                m_usingNepomukDirectly = true;
                Nepomuk::Query::Query myQuery;
                Nepomuk::Query::ComparisonTerm term(Nepomuk::Vocabulary::NMM::performer(), Nepomuk::Query::Term());
                term.setInverted(true);
                Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);
                connect(queryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
                        this, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
                connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
                connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));
                myQuery.setTerm(term);
                kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();
                queryClient->query(myQuery);
                break;
            }
            case 2: {
                m_usingNepomukDirectly = true;
                Nepomuk::Query::Query myQuery;
                Nepomuk::Query::ComparisonTerm term(Nepomuk::Vocabulary::NMM::musicAlbum(), Nepomuk::Query::Term());
                term.setInverted(true);
                Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);
                connect(queryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
                        this, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
                connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
                connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));
                myQuery.setTerm(term);
                kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();
                queryClient->query(myQuery);
                break;
            }
            reset();
        }
        return true;
    }
    return false;
}

#include "metadatamusicmodel.moc"
