/***************************************************************************
 *   Copyright 2009 by Onur-Hayri Bakici   <thehayro@gmail.com>            *
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

#ifndef PICTURE_H
#define PICTURE_H

#include <Plasma/DataEngine>
#include <QHash>

#include "pictureprovider/pictureprovider.h"

namespace KIO {
    class Job;
}

class Picture : public Plasma::DataEngine
{
    Q_OBJECT
public:
    Picture(QObject *parent, const QVariantList &args);
    ~Picture();

protected:
    /**
     * Queries to this dataengine should be in the form:
     * service:searchterm&constraint1=value&constraint2=value ...
     * where everything after the first "&" is optional.
     * the "service" parameter indicates which picture provider to query.
     */
    bool sourceRequestEvent(const QString &source);

    bool updateSourceEvent(const QString &source);

protected slots:
    void dataFromResults(const QString &, const QList<PicturePackage> &pictures);

private:
    PictureProvider *m_currentProvider;
    QHash<KIO::Job*, QString> m_jobs;
};

#endif // PICTURE_H
