/*
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#ifndef VIDEO_H
#define VIDEO_H

#include <Plasma/DataEngine>
#include <QHash>

#include "videoprovider/videoprovider.h"
class KPixmapCache;
namespace KIO {
    class Job;
}

class Video : public Plasma::DataEngine
{
    Q_OBJECT
public:
    Video(QObject *parent, const QVariantList &args);
    ~Video();
    
protected:
    /**
     * Queries to this dataengine should be in the form:
     * service:searchterm&constraint1=value&constraint2=value ...
     * where everything after the first "&" is optional.
     * the "service" parameter indicates which video provider to query.
     */
    bool sourceRequestEvent(const QString &source);

    bool updateSourceEvent(const QString &source);

protected slots:
    void dataFromResults(const QString &, const QList<VideoPackage> &videos);
    void thumbnailReceived(KIO::Job*, const QByteArray &);

private:
    VideoProvider *m_currentProvider;
    KPixmapCache *m_thumbnailsCache;
    QHash<KIO::Job*,QString> m_jobs;
};

#endif
