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

MetadataMusicModel::Category::Category ( const QString& label, const QString& icon, const Nepomuk::Types::Property& nepomukProperty )
    : m_label(label), m_icon(icon), m_property(nepomukProperty)
{
}

QString MetadataMusicModel::Category::icon() const
{
    return m_icon;
}

QString MetadataMusicModel::Category::label() const
{
    return m_label;
}

Nepomuk::Types::Property MetadataMusicModel::Category::nepomukProperty() const
{
    return m_property;
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
    m_categories<< Category("All Songs", "audio", Nepomuk::Vocabulary::NFO::Audio())    //unused NFO:Audio to satisfy the compiler
                << Category("Artists", "view-media-artist", Nepomuk::Vocabulary::NMM::performer())
                << Category("Albums", "tools-media-optical-copy", Nepomuk::Vocabulary::NMM::musicAlbum());
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
        case Qt::DecorationRole:
            if (m_showingCategories) {
                return m_categories.at(index.row()).icon();
            } else {
                return m_categories.at(m_currentCategory).icon();
            }
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
                return true;
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
    m_showingAllSongs = false;
    if (m_showingCategories) {
        m_currentCategory = Categories(row);
        m_showingCategories = false;
        switch(row) {
            case AllSongs:
                m_usingNepomukDirectly = false;
                m_showingAllSongs = true;
                metadataModel()->setProperty("resourceType", "nfo:Audio");
                metadataModel()->setProperty("mimeType", "");
                metadataModel()->setProperty("limit", 500);
                break;
            case Artists:
            case Albums: {
                m_usingNepomukDirectly = true;
                Nepomuk::Query::Query myQuery;
                Nepomuk::Query::ComparisonTerm term(m_categories.at(m_currentCategory).nepomukProperty(), Nepomuk::Query::Term());
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
    } else {
        QString name = data(index(row, 0)).toString();
        m_usingNepomukDirectly = false;

        QDeclarativePropertyMap *map
            = qobject_cast<QDeclarativePropertyMap*>(metadataModel()->property("extraParameters").value<QObject*>());
        map->clear("nmm:performer");
        map->clear("nmm:musicAlbum");
        switch (m_currentCategory) {
            case Artists:
                map->insert("nmm:performer", name);
                break;
            case Albums:
                map->insert("nmm:musicAlbum", name);
                break;
        }
        metadataModel()->setProperty("resourceType", "nfo:Audio");
    }

    return false;
}

bool MetadataMusicModel::goOneLevelUp()
{
    if (m_showingCategories) {
        return false;
    }
    if (!m_usingNepomukDirectly) {
        if (m_showingAllSongs) {
            m_showingCategories = true;
            m_usingNepomukDirectly = false;
            reset();
            return true;
        } else {
            m_showingCategories = true;
            return browseTo(m_currentCategory);
        }
    } else {
        m_showingCategories = true;
        m_usingNepomukDirectly = false;
        reset();
        return true;
    }
    return false;
}

#include "metadatamusicmodel.moc"
