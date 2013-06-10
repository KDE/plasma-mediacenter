/***********************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>            *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

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
    AbstractBrowsingBackendPrivate(AbstractBrowsingBackend *q) :
    q(q),
    cfInterface(false),
    declarativeEngine(0),
    hasInitialized(false)
    {}

    AbstractBrowsingBackend *q;
    bool cfInterface;
    QDeclarativeEngine *declarativeEngine;
    bool hasInitialized;
    QString name;
    QString mediaBrowserSidePanelText;
    QList<QAbstractItemModel*> models;
};

AbstractBrowsingBackend::AbstractBrowsingBackend(QObject *parent, const QVariantList &args)
    : QObject(parent),
      d(new AbstractBrowsingBackendPrivate(this))
{
    Q_UNUSED(args);
    qRegisterMetaType<QAbstractItemModel*>("QAbstractItemModel*");
    d->mediaBrowserSidePanelText.clear();
}

AbstractBrowsingBackend::~AbstractBrowsingBackend()
{
    delete d;
}

void AbstractBrowsingBackend::setName(const QString &name)
{
    d->name = name;
}

QString AbstractBrowsingBackend::name() const
{
    if (d->name.isEmpty()) {
        return "generic-backend";
    }

    return d->name;
}

void AbstractBrowsingBackend::setModel(QAbstractItemModel * model)
{
    d->models.clear();
    d->models.append(model);
    emit modelChanged();
}

QObject * AbstractBrowsingBackend::model()
{
    return d->models.length() ? (QObject*)(d->models.first()) : 0;
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

bool AbstractBrowsingBackend::expand(int row)
{
    Q_UNUSED(row)
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
    return QString("import QtQuick 1.1\nimport org.kde.plasma.mediacenter.elements.%1 %2 as %3\n\%3.%4 {\n}\n")
        .arg(componentDirName).arg(versionString).arg(componentDirName.toUpper()).arg(itemName);
}

QString AbstractBrowsingBackend::mediaBrowserOverride() const
{
    return QString();
}

QString AbstractBrowsingBackend::mediaBrowserSidePanel() const
{
    return d->mediaBrowserSidePanelText;
}

void AbstractBrowsingBackend::setMediaBrowserSidePanel(QString text)
{
    d->mediaBrowserSidePanelText = text;
    emit mediaBrowserSidePanelChanged();
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
    Q_UNUSED(searchTerm)
    // Does nothing
}

bool AbstractBrowsingBackend::init()
{
    if (!d->hasInitialized) {
        d->hasInitialized = initImpl();
    }
    return d->hasInitialized;
}

bool AbstractBrowsingBackend::busy() const
{
    return false;
}
