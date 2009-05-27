/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "youtube.h"
#include "youtubeinterface.h"

// KDE
#include <kio/job.h>


YouTubeEngine::YouTubeEngine(QObject *parent, const QVariantList &args) : Plasma::DataEngine(parent, args),
m_interface(new YouTubeInterface(this))
{
    connect (m_interface, SIGNAL(result(QString,QString,Plasma::DataEngine::Data)), this, SLOT(slotSourceFromResult(QString,QString,Plasma::DataEngine::Data)));
}

YouTubeEngine::~YouTubeEngine()
{
}

bool YouTubeEngine::sourceRequestEvent(const QString &name)
{
    if (!name.startsWith("query/")) {
        return false;
    }
    QString queryString = name;
    queryString.remove("query/");
    m_interface->query(queryString);

    return true;
}

void YouTubeEngine::slotSourceFromResult(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &video)
{
    setData(searchTerm, id, video);
}

K_EXPORT_PLASMA_DATAENGINE(youtube, YouTubeEngine)
