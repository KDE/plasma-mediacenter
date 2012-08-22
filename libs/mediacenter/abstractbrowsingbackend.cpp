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
#include <KServiceTypeTrader>

#include <QAbstractItemModel>
#include <QDeclarativeEngine>

using namespace MediaCenter;

class AbstractBrowsingBackend::AbstractBrowsingBackendPrivate
{
public:
    AbstractBrowsingBackendPrivate(KService::Ptr service, AbstractBrowsingBackend *q) :
    backendInfo(service),
    q(q),
    cfInterface(false),
    model(0),
    metadataModel(0),
    declarativeEngine(0)
    {}

    AbstractBrowsingBackend *q;
    bool cfInterface;
    KPluginInfo backendInfo;
    BrowsingType browsingType;
    QAbstractItemModel * model;
    QAbstractItemModel * metadataModel;
    QDeclarativeEngine *declarativeEngine;
};

AbstractBrowsingBackend::AbstractBrowsingBackend(QObject *parent, const QVariantList &args) : QObject(parent),
d(new AbstractBrowsingBackendPrivate(KService::serviceByStorageId(args.count() ? args.first().toString() : QString()), this))
{
    Q_UNUSED(args);
    qRegisterMetaType<QAbstractItemModel*>("QAbstractItemModel*");
}

AbstractBrowsingBackend::AbstractBrowsingBackend(QObject * parent)
  : QObject(parent),
    d(new AbstractBrowsingBackendPrivate(KService::serviceByStorageId(QString()), this))
{
}

AbstractBrowsingBackend::~AbstractBrowsingBackend()
{
    delete d;
}

void AbstractBrowsingBackend::setModel(QAbstractItemModel * model)
{
    d->model = model;
    emit modelChanged();
}

QObject * AbstractBrowsingBackend::model()
{
    return (QObject*)(d->model);
}

QObject* AbstractBrowsingBackend::metadataModel()
{
    return (QObject*)(d->metadataModel);
}

void AbstractBrowsingBackend::setMetadataModel (QObject* model)
{
    d->metadataModel = qobject_cast<QAbstractItemModel*>(model);
    emit metadataModelChanged();
}

bool AbstractBrowsingBackend::hasConfigurationInterface() const
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
    return KConfigGroup(KGlobal::config(), name());
}

void AbstractBrowsingBackend::init()
{}

QString AbstractBrowsingBackend::name() const
{
    return d->backendInfo.name();
}

QString AbstractBrowsingBackend::icon() const
{
    return d->backendInfo.icon();
}

AbstractBrowsingBackend::BrowsingType AbstractBrowsingBackend::browsingType() const
{
    return d->browsingType;
}

void AbstractBrowsingBackend::setBrowsingType(BrowsingType type)
{
    d->browsingType = type;
}

void AbstractBrowsingBackend::openUrl(const KUrl &url)
{
    Q_UNUSED(url);
}

KService::List AbstractBrowsingBackend::availableBackends()
{
    KService::List plugins = KServiceTypeTrader::self()->query("Plasma/MediaCenter/BrowsingBackend");
    if (plugins.isEmpty()) {
        kWarning() << "no available browsing backend";
    }
    return plugins;
}

bool AbstractBrowsingBackend::goOneLevelUp()
{
    return false;
}

bool AbstractBrowsingBackend::expand (int row)
{
    return false;
}

QDeclarativeEngine *AbstractBrowsingBackend::declarativeEngine() const
{
    return d->declarativeEngine;
}

void AbstractBrowsingBackend::setDeclarativeEngine(QDeclarativeEngine *declarativeEngine)
{
    d->declarativeEngine = declarativeEngine;
}

QString AbstractBrowsingBackend::constructQmlSource(const QString& componentDirName, const QString &versionString,
                                                    const QString& itemName) const
{
    return QString("import QtQuick 1.1\n\import org.kde.plasma.mediacentercomponents.%1 %2 as %3\n\%3.%4 {\n\}\n")
        .arg(componentDirName).arg(versionString).arg(componentDirName.toUpper()).arg(itemName);
}

QString AbstractBrowsingBackend::mediaBrowserOverride() const
{
    return QString();
}

QString AbstractBrowsingBackend::mediaBrowserSidePanel() const
{
    return QString();
}

bool AbstractBrowsingBackend::okToLoad() const
{
    return true;
}

bool AbstractBrowsingBackend::supportsSearch() const
{
    return false;
}

void AbstractBrowsingBackend::search(const QString& searchTerm)
{
    // Does nothing
}
