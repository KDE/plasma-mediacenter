/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
 *   Copyright 2012 Sinny Kumari <ksinny@gmail.com>                        *
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

#ifndef FLICKRMODEL_H
#define FLICKRMODEL_H

#include <QAbstractListModel>

namespace KIO {
    class Job;
}

class KJob;

class FlickrModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit FlickrModel (QObject* parent = 0);
    ~FlickrModel();
    void query(const QString &searchTerm);

    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

protected slots:
    void flickrDataReady(KIO::Job *job, const QByteArray &data);
    void parseResults(KJob *job);

private:
    struct Photo;
    QHash<KIO::Job*, QString> m_queries;
    QHash<KIO::Job*, QString> m_datas;
    QList<Photo> m_photos;

    void listPhotos(KJob *job);
};

struct FlickrModel::Photo
{
public:
    QString title;
    QString id;
    QString owner;
    QString secret;
    QString farmID;
    QString serverID;
    QString previewImgLink;
    QString originalImgLink;
};

#endif // FLICKRMODEL_H
