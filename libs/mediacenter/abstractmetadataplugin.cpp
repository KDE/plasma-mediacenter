/*
 *   Copyright 2015 Ashish Bansal <bansal.ashish096@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
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

#include "abstractmetadataplugin.h"

using namespace MediaCenter;

class AbstractMetadataPlugin::Private
{
public:
    Private() {}
    QString pluginName;
};

AbstractMetadataPlugin::AbstractMetadataPlugin(QObject* parent, const QVariantList&)
    : QObject(parent), d(new Private)
{
}

AbstractMetadataPlugin::~AbstractMetadataPlugin()
{
    delete d;
}

QString AbstractMetadataPlugin::getPluginName()
{
    return d->pluginName;
}

void AbstractMetadataPlugin::setPluginName(QString pluginName)
{
    d->pluginName = pluginName;
}
