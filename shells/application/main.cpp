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
#include <qcommandlineparser.h>
#include <qcommandlineoption.h>

#include <KLocalizedString>

#include "pmcview.h"
#include "pmccorona.h"

#include "plasmaquick/shellpluginloader.h"

static const char version[] = "1.0";

int main(int argc, char **argv)
{
    QQuickWindow::setDefaultAlphaBuffer(true);

    QApplication app(argc, argv);
    app.setApplicationVersion(version);

    QCommandLineParser parser;
    parser.setApplicationDescription(i18n("Plasma Media Center"));
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(app);

    Plasma::Theme theme;
    theme.setUseGlobalSettings(false);
    theme.setThemeName("breeze-dark");

    Plasma::PluginLoader::setPluginLoader(new ShellPluginLoader);

    PmcCorona *corona = new PmcCorona();

    return app.exec();
}
