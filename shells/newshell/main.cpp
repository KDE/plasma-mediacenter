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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include <Plasma/Theme>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    KAboutData aboutData("plasma-mediacenter",
                         0,
                         ki18n("Plasma Mediacenter Shell"),
                         "0.1",
                         ki18n("A convenient shell for the Plasma Media Center Components"),
                         KAboutData::License_GPL_V2,
                         ki18n("Copyright (c) 2009-2011"));
    aboutData.addAuthor(ki18n("Alessandro Diaferia"),
                        ki18n("Mantainer and main developer"),
                        "alediaferia@gmail.com",
                        "http://alediaferia.wordpress.com");
    aboutData.addAuthor(ki18n("Sinny Kumari"),
                        ki18n("Mantainer and main developer"),
                        "ksinny@gmail.com",
                        "http://www.sinny.in");
    aboutData.addCredit(ki18n("Marco Martin"), ki18n("GSoC project mentor"), "notmart@gmail.com", "");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add("fullscreen", ki18n("Starts Plasma Media Center in fullscreen mode (the default)"));
    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;

    MainWindow *mw = new MainWindow;
    mw->show();
    return app.exec();
}
