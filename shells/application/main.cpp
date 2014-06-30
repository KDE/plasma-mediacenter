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

#include <QApplication>

#include <Plasma/Package>
#include <Plasma/PluginLoader>

#include "pmcview.h"
#include "pmccorona.h"

static const char version[] = "5.0";

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    Plasma::Package package = Plasma::PluginLoader::self()->loadPackage("Plasma/Shell");
    package.setPath("org.kde.plasma.mediacenter");

    PmcCorona *corona = new PmcCorona();
    corona->setPackage(package);
    corona->load();

    PmcView *view = new PmcView(corona);
    view->show();

    return app.exec();
}
