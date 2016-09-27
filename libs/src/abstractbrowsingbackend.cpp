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
#include "objectpair.h"
#include "pmcmodel.h"

#include <KPluginInfo>
#include <QDebug>


using namespace KMediaCollection;

class AbstractBrowsingBackend::AbstractBrowsingBackendPrivate
{
public:
    AbstractBrowsingBackendPrivate() :
    hasInitialized(false)
    {}

    KPluginInfo pluginInfo;
    bool hasInitialized;
    QList<QObject*> models;
    QString searchTerm;
    QStringList buttons;
};

AbstractBrowsingBackend::AbstractBrowsingBackend(QObject *parent, const QVariantList &args)
    : QObject(parent),
      d(new AbstractBrowsingBackendPrivate)
{
    Q_UNUSED(args);
    qRegisterMetaType<QAbstractItemModel*>("QAbstractItemModel*");
}

AbstractBrowsingBackend::~AbstractBrowsingBackend()
{
    delete d;
}

QString AbstractBrowsingBackend::name() const
{
    if (d->pluginInfo.isValid()) {
        return d->pluginInfo.name();
    }
    return "generic-backend";
}

QString AbstractBrowsingBackend::icon() const
{
    if (d->pluginInfo.isValid()) {
        return d->pluginInfo.icon();
    }
    return "applications-multimedia";
}

QString AbstractBrowsingBackend::comment() const
{
    if (d->pluginInfo.isValid()) {
        return d->pluginInfo.comment();
    }
    return "Generic Backend";
}

QString AbstractBrowsingBackend::category() const
{
    if (d->pluginInfo.isValid()) {
        return d->pluginInfo.category();
    }
    return "general";
}

void AbstractBrowsingBackend::setModel(PmcModel* model)
{
    d->models.clear();
    addModel(model);
}

void AbstractBrowsingBackend::setPluginInfo(const KPluginInfo& info)
{
    d->pluginInfo = info;
}

void AbstractBrowsingBackend::setModel(QAbstractItemModel* model)
{
    PmcModel *metadata = new PmcModel(model, this);
    setModel(metadata);
}

void AbstractBrowsingBackend::addModel(PmcModel* model)
{
    //TODO: Implement multiple models
    //d->modelsInBackend.addModel(model);
    d->models.append(model);
}

bool AbstractBrowsingBackend::replaceModel(PmcModel* original,
                                           PmcModel* replacement)
{
    Q_UNUSED(original)
    Q_UNUSED(replacement)
    //TODO: Implement replaceModel
    //return d->modelsInBackend.replaceModel(original, replacement);
    return true;
}

QAbstractItemModel* AbstractBrowsingBackend::model()
{
     QObject *model = d->models.length() ? (QObject*)(d->models.first()) : 0;
     if (model) {
         return qobject_cast<QAbstractItemModel*>(qobject_cast<PmcModel*>(model)->model());
     }
     return 0;
}

bool AbstractBrowsingBackend::goOneLevelUp()
{
    return false;
}

bool AbstractBrowsingBackend::back(QObject* model)
{
    Q_UNUSED(model)
    return false;
}

bool AbstractBrowsingBackend::expand(int row, QAbstractItemModel* model)
{
    Q_UNUSED(model)
    return expand(row);
}

bool AbstractBrowsingBackend::expand(int row)
{
    Q_UNUSED(row)
    return false;
}

QString AbstractBrowsingBackend::constructQmlSource(const QString& componentDirName, const QString &versionString,
                                                    const QString& itemName) const
{
    return QString("import QtQuick 2.1\nimport org.kde.plasma.mediacollection.elements.%1 %2 as %3\n\%3.%4 {\n}\n")
        .arg(componentDirName).arg(versionString).arg(componentDirName.toUpper()).arg(itemName);
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

//TODO: implement multiple models
//QObject* AbstractBrowsingBackend::models()
//{
//    //return &d->modelsInBackend;
//}

QStringList AbstractBrowsingBackend::buttons() const
{
    return d->buttons;
}

QString AbstractBrowsingBackend::viewType() const
{
     QObject *model = d->models.length() ? (QObject*)(d->models.first()) : 0;
     if (model) {
         return qobject_cast<PmcModel*>(model)->viewType();
     }
     return QString();
}

void AbstractBrowsingBackend::setButtons(const QStringList& buttons)
{
    if (d->buttons != buttons) {
        d->buttons = buttons;
        emit buttonsChanged();
    }
}

void AbstractBrowsingBackend::handleButtonClick(const QString&)
{
}

QStringList AbstractBrowsingBackend::allMedia()
{
    return QStringList();
}
