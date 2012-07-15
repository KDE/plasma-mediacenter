/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2012 Shantanu Tushar <shaan7in@gmail.com>                   *
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

#ifndef NEPOMUKMUSICMODEL_H
#define NEPOMUKMUSICMODEL_H

#include <QAbstractListModel>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Types/Property>
#include <nepomuk/term.h>

class NepomukMusicModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit NepomukMusicModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    void setTerm(Nepomuk::Query::Term term, const QString& iconName);
    void setResourceType(Nepomuk::Types::Property property, const QString& iconName);

protected Q_SLOTS:
    void newEntries(const QList< Nepomuk::Query::Result > &entries);
    void entriesRemoved(QList< QUrl > entries);
    void finishedListing();
    void updateModel();

private:
    QString m_icon;
    QList< Nepomuk::Query::Result > m_queryResults;
    Nepomuk::Query::Term m_term;
};

#endif // NEPOMUKMUSICMODEL_H
