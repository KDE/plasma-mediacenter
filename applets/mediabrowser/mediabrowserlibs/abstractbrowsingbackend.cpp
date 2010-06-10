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
#include "abstractbrowsingbackend.h"

#include <KGlobal>
#include <KService>
#include <KPluginInfo>
#include <KDebug>

class AbstractBrowsingBackend::AbstractBrowsingBackendPrivate
{
public:
    AbstractBrowsingBackendPrivate(KService::Ptr service, AbstractBrowsingBackend *q) :
    backendInfo(service),
    q(q),
    cfInterface(false)
    {}

    AbstractBrowsingBackend *q;
    bool cfInterface;
    KPluginInfo backendInfo;
    MediaCenter::MediaTypes allowedMediaTypes;
};

AbstractBrowsingBackend::AbstractBrowsingBackend(QObject *parent, const QVariantList &args) : QObject(parent),
d(new AbstractBrowsingBackendPrivate(KService::serviceByStorageId(args.count() ? args.first().toString() : QString()), this))
{
    Q_UNUSED(args);
}

AbstractBrowsingBackend::~AbstractBrowsingBackend()
{
}

bool AbstractBrowsingBackend::hasConfigurationInterface()
{
    return d->cfInterface;
}

void AbstractBrowsingBackend::setHasConfigurationInterface(bool hasInterface)
{
    d->cfInterface = hasInterface;
}

void AbstractBrowsingBackend::createConfigurationInterface(KConfigDialog *parent)
{
    Q_UNUSED(parent);
}

KConfigGroup AbstractBrowsingBackend::config()
{
    kDebug() << KGlobal::config()->name();
    return KConfigGroup(KGlobal::config(), name());
}

void AbstractBrowsingBackend::init()
{}

void AbstractBrowsingBackend::setAllowedMediaTypes(const MediaCenter::MediaTypes &type)
{
    d->allowedMediaTypes = type;
}

MediaCenter::MediaTypes AbstractBrowsingBackend::allowedMediaTypes()
{
    return d->allowedMediaTypes;
}

QString AbstractBrowsingBackend::name() const
{
    return d->backendInfo.name();
}

