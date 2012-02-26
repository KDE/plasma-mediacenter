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

#ifndef METADATAMUSICMODEL_H
#define METADATAMUSICMODEL_H

#include "../abstractmetadatamodel.h"
#include <QAbstractItemModel>

#include <Nepomuk/Query/Result>
#include <Nepomuk/Types/Property>

class MetadataMusicModel : public AbstractMetadataModel
{
    Q_OBJECT
protected Q_SLOTS:
    void newEntries(const QList< Nepomuk::Query::Result > &entries);
    void finishedListing();
    void error(const QString& e);
public:
    explicit MetadataMusicModel (QObject* parent = 0);
    virtual ~MetadataMusicModel();

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    bool browseTo(int row);
    virtual bool goOneLevelUp();

private:
    class Category;
    enum Categories { AllSongs, Artists, Albums };

    QList< Nepomuk::Query::Result > m_queryResults;
    QList< Category > m_categories;
    bool m_showingCategories;
    bool m_usingNepomukDirectly;
    bool m_showingAllSongs;     //FIXME: Implement a state machine to get rid of crap
    Categories m_currentCategory;

    void initializeCategories();
    bool m_browseTo;
};

class MetadataMusicModel::Category
{
public:
    Category(const QString &label, const QString &icon, const Nepomuk::Types::Property &nepomukProperty);
    QString label() const;
    QString icon() const;
    Nepomuk::Types::Property nepomukProperty() const;
    
private:
    QString m_label;
    QString m_icon;
    Nepomuk::Types::Property m_property;
};

#endif // METADATAMUSICMODEL_H
