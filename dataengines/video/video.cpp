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
#include "video.h"

#include <KServiceTypeTrader>
#include <KService>

Video::Video(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args),
m_currentProvider(0)
{}

Video::~Video()
{}

bool Video::sourceRequestEvent(const QString &source)
{
    if (!source.contains(':')) {
        return false;
    }
    
    const QStringList parms = source.split(':');
    const QString serviceName = parms[0];
    
    if (!m_currentProvider || m_currentProvider->serviceId() != serviceName) {
        delete m_currentProvider;
        KService::List offers = KServiceTypeTrader::self()->query("Plasma/MediaCenter/VideoProvider");
        KService::Ptr service;
        KService::List::const_iterator it = offers.constBegin();
        KService::List::const_iterator end = offers.constEnd();
        for (; it != end; ++it) {
            if ((*it)->name() == serviceName) {
                service = *it;
                break;
            }
        }
        
        if (service.isNull()) {
            return false;
        }
        
        m_currentProvider = service->createInstance<VideoProvider>(this);
        connect(m_currentProvider, SIGNAL(searchResult(QString, QList<VideoPackage>)),
                this, SLOT(dataFromResults(QString, QList<VideoPackage)));
    }
    
    // TODO: use only the queryied string as source name, not also the eventual params
    setData(parms[1], Plasma::DataEngine::Data());
    
    // TODO: parse arguments in the query and pass separatedly
    m_currentProvider->searchByQuery(parms[1]);
    return true;    
}

void Video::dataFromResults(const QString &query, const QList<VideoPackage> &videos)
{}

K_EXPORT_PLASMA_DATAENGINE(video, Video)
