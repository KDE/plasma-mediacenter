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

#ifndef ABSTRACTPLUGIN_H
#define ABSTRACTPLUGIN_H

#include <KPluginInfo>

#include "mediacenter_export.h"

#include <QObject>

namespace MediaCenter {
class MEDIACENTER_EXPORT AbstractPlugin : public QObject {
    Q_OBJECT

public:
    explicit AbstractPlugin(QObject* parent = 0, const QVariantList& = QVariantList());
    ~AbstractPlugin();
    QString getPluginName();
    void setPluginName(QString pluginName);
private:
    class Private;
    Private * const d;
};
}

#define MEDIACENTER_EXPORT_PLUGIN(classname, jsonfile) \
    K_PLUGIN_FACTORY_WITH_JSON( PluginFactory, jsonfile, registerPlugin< classname >(); )

#endif // ABSTRACTPLUGIN_H
