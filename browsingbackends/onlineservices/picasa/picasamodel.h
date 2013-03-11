/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
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
#ifndef PICASAMODEL_H
#define PICASAMODEL_H

#include <QObject>
#include <Plasma/DataEngine>
#include <QHash>
#include <QAbstractListModel>

namespace KIO {
    class Job;
}
class QByteArray;
class KJob;

/**
 * @class PicasaModel
 * @brief Model to provide data to required to the QML gridView
 * @author Francesco Grieco
 *
 * This class interfaces Picasa and returns result entries
 * with the signal result.
 */

class PicasaModel: public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PicasaModel(QObject* parent, const QString& username, const QString& password);
    virtual ~PicasaModel();
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

   void query(const QString &searchTerm, const QString &request);
   void getTokenAndQuery(const QString &username, const QString &password, const QString &request);
   bool browseToAlbum(int row);
   bool goBack();

signals:
    /**
     * @return the entry as Plasma::DataEngine::Data.
     * each value can be retrieved using the following keys:
     * "title" for the video title
     * "description" for the video description
     * "keywords" for the video keywords
     *
     * @param searchTerm is the search term to be used as source name.
     * @param id is the album/photo id to be used as source key
     * @param image is the result entry retrieved after the search.
     */
    void result(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &image);
    void loginComplete(bool status);

protected slots:
    void picasaDataReady(KIO::Job *job, const QByteArray &data);
    void parseResults(KJob *job);
    void passwordJob(KJob *job);
    void token(KIO::Job *job, const QByteArray &data);

private:
    QHash<KIO::Job*, QString> m_queries;
    QHash<KIO::Job*, QString> m_datas;

    struct Album;
    struct Photo;
    QList<Album> m_albums;
    QList<Photo> m_photos;
    QString m_token;
    QString m_request;
    QString m_username;
    QString m_albumid;
    bool m_expandable;
    bool m_flag;       //to check whether valid data is present in picasa plugin or not

    void listAllAlbums(KJob *job);
    void listAllPhotos(KJob *job);

};

struct PicasaModel::Photo {
public:
    QString published;
    QString updated;
    QString title;
    QString link;
    QString albumId;
    QString width;
    QString height;
    QString size;
    QString thumbnail72;
    QString thumbnail144;
    QString thumbnail288;
};

struct PicasaModel::Album {
public:
    QString id;
    QString published;
    QString updated;
    QString title;
    QString link;
    QString summary;
    QString noOfPhotos;
    QString thumbnail;
};

#endif
