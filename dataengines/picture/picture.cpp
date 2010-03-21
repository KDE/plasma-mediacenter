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

#include "picture.h"

#include <KServiceTypeTrader>
#include <KService>
#include <kio/job.h>
#include <kdebug.h>


Picture::Picture(QObject *parent, const QVariantList &args)
        : Plasma::DataEngine(parent, args),
        m_currentProvider(0)
{}

Picture::~Picture()
{}

bool Picture::sourceRequestEvent(const QString &source)
{
    if (source == "providers")
    {
        KService::List offers = KServiceTypeTrader::self()->query("Plasma/MediaCenter/PictureProvider");
        QStringList providers;
        foreach (KService::Ptr service, offers)
        {
            providers << service->property("X-KDE-PictureProvider-Name", QVariant::String).toString();
            //qDebug() << service->property("X-KDE-PictureProvider-Name", QVariant::String).toString();
        }
        setData(source, providers);
        return true;
    }

    if (!source.contains(':'))
        return false;

    const QStringList params = source.split(':');
    const QString serviceName = params[0];

    if (!m_currentProvider || m_currentProvider->serviceId() != serviceName)
    {
        delete m_currentProvider;
        m_currentProvider = 0;
        KService::List offers = KServiceTypeTrader::self()->query("Plasma/MediaCenter/PictureProvider");
        KService::Ptr service;
        KService::List::const_iterator it = offers.constBegin();
        KService::List::const_iterator end = offers.constEnd();

        for (; it != end; ++it)
        {
            if ((*it)->property("X-KDE-PictureProvider-Name", QVariant::String).toString() == serviceName)
            {
                service = *it;
                break;
            }
        }

        if (service.isNull())
            return false;

        m_currentProvider = service->createInstance<PictureProvider>(this);
        connect(m_currentProvider, SIGNAL(searchResult(QString,QList<PicturePackage>)),
                this, SLOT(dataFromResults(QString,QList<PicturePackage>)));
    }

    // Optional arguments
    QStringList args = params[1].split("&");
    QString query = args.takeFirst();
    QVariantList constraints;
    foreach (const QString &arg, args)
    {
        QStringList pieces = arg.split('=');
        if (pieces.count() != 2)
            continue;
        constraints << pieces[0] << pieces[1];
    }

    setData(params[1], Plasma::DataEngine::Data());
    m_currentProvider->query(query, constraints);
    return true;
}

void Picture::dataFromResults(const QString &query, const QList<PicturePackage> &pictures)
{
    this->removeAllSources();
    QStringList ids;
    QList<PicturePackage>::const_iterator it = pictures.begin();
    QList<PicturePackage>::const_iterator end = pictures.end();
    for (; it != end; ++it)
    {
        ids << it->id;
        Plasma::DataEngine::Data pictureData;

        pictureData["author"] = it->author;
        pictureData["id"] = it->id;
        pictureData["title"] = it->title;
        pictureData["published"] = it->published;
        pictureData["updated"] = it->updated;
        pictureData["link"] = it->link;
        pictureData["width"] = it->width;
        pictureData["height"] = it->height;
        pictureData["size"] = it->size;

        pictureData["albumId"] = it->albumId;
        pictureData["keywords"] = it->keywords;
        setData(it->id, pictureData);
    }

    Q_ASSERT(sources().contains(query));
    setData(query, ids);

}

bool Picture::updateSourceEvent(const QString &source)
{
    return true;
}

K_EXPORT_PLASMA_DATAENGINE(picture, Picture)
