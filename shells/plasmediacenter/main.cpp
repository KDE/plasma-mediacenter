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
    KAboutData aboutData("plasmediacenter",
                         0,
                         ki18n("Plasmediacenter"),
                         "0.1",
                         ki18n("A convenient shell for the Plasma Media Center Components"),
                         KAboutData::License_GPL_V2,
                         ki18n("Copyright (c) 2009"));
    aboutData.addAuthor(ki18n("Alessandro Diaferia"),
                        ki18n("Mantainer and main developer"),
                        "alediaferia@gmail.com",
                        "http://alediaferia.wordpress.com");

    KCmdLineArgs::init(&aboutData);

    KApplication app;

    Plasma::Theme::defaultTheme()->setUseGlobalSettings(false);

    MainWindow *mw = new MainWindow;
    mw->show();
    mw->loadMediaCenter();
    return app.exec();

}
