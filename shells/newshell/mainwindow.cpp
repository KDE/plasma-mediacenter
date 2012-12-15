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

#include "mainwindow.h"

#include <libs/mediacenter/backendsmodel.h>
#include <libs/mediacenter/playlistmodel.h>
#include <libs/mediacenter/filteredbackendsmodel.h>
#include <libs/mediacenter/subtitleprovider.h>
#include <libs/mediacenter/pmcimageprovider.h>
#include <libs/mediacenter/pmccoverartprovider.h>

#include <Plasma/Package>
#include <KDE/KCmdLineArgs>

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <qdeclarative.h>

#ifdef QT_MULTIMEDIA_KIT_FOUND
#include <QtMultimediaKit/QVideoWidget>
#endif

#include <QtOpenGL/QGLWidget>
#include <QApplication>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent)
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->isSet("fullscreen")) {
        toggleFullScreen();
    }

    QDeclarativeView *view = new QDeclarativeView(this);

    if (!args->isSet("disable-opengl")) {
        QGLWidget *glWidget = new QGLWidget;
        glWidget->setAutoFillBackground(false);
        view->setViewport(glWidget);
    } else {
#ifdef QT_MULTIMEDIA_KIT_FOUND
        QVideoWidget *widget = new QVideoWidget;
        widget->show();
        view->setViewport(widget);
#endif
    }
    args->clear();

    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setCentralWidget(view);

    m_kdeclarative.setDeclarativeEngine(view->engine());
    m_kdeclarative.initialize();
    m_kdeclarative.setupBindings();

    qmlRegisterType<FilteredBackendsModel>("org.kde.plasma.mediacentercomponents", 0, 1, "FilteredBackendsModel");
    qmlRegisterType<SubtitleProvider>("org.kde.plasma.mediacentercomponents", 0, 1, "SubtitleProvider");
    BackendsModel *backendsModel = new BackendsModel(view->engine(), this);
    view->rootContext()->setContextProperty("backendsModel", backendsModel);
    PlaylistModel *playlistModel = new PlaylistModel(this);
    view->rootContext()->setContextProperty("playlistModel", playlistModel);

    view->rootContext()->setContextProperty("mainwindow", this);
    view->rootContext()->setContextProperty("startedFullScreen", isFullScreen());

    view->engine()->addImageProvider(PmcImageProvider::identificationString, new PmcImageProvider());
    view->engine()->addImageProvider(PmcCoverArtProvider::identificationString, new PmcCoverArtProvider());

    m_structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", m_structure);

    view->rootContext()->setContextProperty("_pmc_background_image_path", QUrl(package->filePath("images", "noiselight.png")));
    view->rootContext()->setContextProperty("_pmc_gradient_image_path", QUrl(package->filePath("images", "gradient.png")));
    view->rootContext()->setContextProperty("_pmc_shadow_image_path", QUrl(package->filePath("images", "shadow.png")));

    view->setSource(QUrl(package->filePath("mainscript")));

    resize(1024, 768);
}

bool MainWindow::toggleFullScreen()
{
    if (windowState() & Qt::WindowFullScreen) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
    }
    return (windowState() & Qt::WindowFullScreen);
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeMediaCenter()
{
    QApplication::quit();
}
