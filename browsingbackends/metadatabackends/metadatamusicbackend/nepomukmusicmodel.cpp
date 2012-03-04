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


NepomukMusicModel::NepomukMusicModel(QObject* parent): QAbstractItemModel(parent)
{
    m_categoryData.append(CategoryData("audio", Nepomuk::Vocabulary::NFO::Audio()));
    m_categoryData.append(CategoryData("view-media-artist", Nepomuk::Vocabulary::NMM::performer()));
    m_categoryData.append(CategoryData("tools-media-optical-copy", Nepomuk::Vocabulary::NMM::musicAlbum()));
}

void NepomukMusicModel::setTerm(Nepomuk::Types::Property term)
{
    Nepomuk::Query::Query myQuery;
    kDebug() << "haaaaaaaaaahaaaaaaaaaaaaaaa";
    Nepomuk::Query::ComparisonTerm ct(term, Nepomuk::Query::Term());
    ct.setInverted(true);
    Nepomuk::Query::QueryServiceClient *queryClient = new Nepomuk::Query::QueryServiceClient(this);
    connect(queryClient, SIGNAL(newEntries(const QList<Nepomuk::Query::Result> &)),
            this, SLOT(newEntries(const QList<Nepomuk::Query::Result> &)));
    connect(queryClient, SIGNAL(error(QString)), SLOT(error(QString)));
    connect(queryClient, SIGNAL(finishedListing()), this, SLOT(finishedListing()));
    myQuery.setTerm(ct);
    kDebug()<< "Sparql query:"<< myQuery.toSparqlQuery();
    queryClient->query(myQuery);
}

QVariant NepomukMusicModel::data(const QModelIndex& index, int role) const
{

    switch(role) {
    case Qt::DecorationRole:
                return "view-media-artist";
        case Qt::DisplayRole:
                return m_queryResults.at(index.row()).resource().genericLabel();
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
    kDebug() << "ITEEM";
      Q_FOREACH (Nepomuk::Query::Result res, entries) {
        m_queryResults.append(res);
    }
}

void NepomukMusicModel::finishedListing()
{
    kDebug() << "FINEESH";
    reset();
}

CategoryData::CategoryData(QString icon, Nepomuk::Types::Property property)
{

    m_property = property;
    m_icon = icon;
}

QString CategoryData::icon() const
{

    return m_icon;
}

Nepomuk::Types::Property CategoryData::property() const
{

    return m_property;
}

