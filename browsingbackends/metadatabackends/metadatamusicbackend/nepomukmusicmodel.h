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

#ifndef NEPOMUKMUSICMODEL_H
#define NEPOMUKMUSICMODEL_H

#include <QAbstractItemModel>
#include <Nepomuk/Query/Result>
#include <Nepomuk/Types/Property>

class NepomukMusicModel : public QAbstractItemModel
{

    Q_OBJECT
    protected Q_SLOTS:
    void newEntries(const QList< Nepomuk::Query::Result > &entries);
    void finishedListing();
public:
    explicit NepomukMusicModel(QObject* parent = 0);
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    void setTerm(Nepomuk::Types::Property term, const QString& iconName);

private:
    QString m_icon;
    QList< Nepomuk::Query::Result > m_queryResults;
};

#endif // NEPOMUKMUSICMODEL_H
