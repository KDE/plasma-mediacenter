/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici <thehayro@gmail.com               *
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


#ifndef FLICKRINTERFACE_H
#define FLICKRINTERFACE_H

#include <QObject>
#include <Plasma/DataEngine>
#include <QHash>

#include <pictureprovider/pictureprovider.h>

namespace KIO {
    class Job;
}

class QByteArray;
class KJob;

/**
  * @class FlickrInterface
  * @brief The interface that queries Flickr with searchTerms
  * @author Onur-Hayri Bakici
  *
  * This class interfaces Flickr and returns result entries
  * with the signal result.
  */

class FlickrInterface : public QObject
{
    Q_OBJECT

public:
    FlickrInterface(QObject *parent = 0);
    ~FlickrInterface();

    void query(const QString &searchTerm);



signals:
    /**
      * @return the entry as Plasma::DataEnginge::Data
      * each value can be retrieved by using the following key
      * "title" for the title of the picture

      * @param searchTerm ist the term to search for
      */
    void result(const QString &searchTerm, const QList<PicturePackage> &pictures);

protected slots:
    void flickrDataReady(KIO::Job *job, const QByteArray &data);
    void parseResults(KJob *job);

private:
    QHash<KIO::Job*, QString> m_queries;
    QHash<KIO::Job*, QString> m_datas;

    void listPhotos(KJob *job);
};


#endif //FLICKRINTERFACE_J
