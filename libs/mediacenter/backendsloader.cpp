/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "backendsloader.h"

#include <QDebug>

#include <KPluginTrader>

class BackendsLoader::Private
{
public:
    QHash<QString, MediaCenter::AbstractBrowsingBackend*> backends;
};

BackendsLoader::BackendsLoader(QObject* parent)
: d(new Private)
{
    const KPluginInfo::List backendsInfo = KPluginTrader::self()->query("plasma/mediacenter/browsingbackends");
    if (backendsInfo.isEmpty()) {
        qWarning() << "No backends found, check your installation";
        return;
    }

    for (auto info : backendsInfo)
    {
        KPluginLoader loader(info.libraryPath());
        KPluginFactory* factory = loader.factory();

        if(factory) {
            MediaCenter::AbstractBrowsingBackend *backend = factory->create<MediaCenter::AbstractBrowsingBackend>(this);
            if(backend) {
                backend->setPluginInfo(info);
                d->backends.insert(info.pluginName(), backend);
            } else {
                qWarning() << "Backend " << info.pluginName() << " could not be loaded";
            }
        }
    }
}

MediaCenter::AbstractBrowsingBackend* BackendsLoader::backend (const QString& backendPluginName)
{
    return d->backends.value(backendPluginName);
}
