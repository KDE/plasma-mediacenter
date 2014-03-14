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
#include "settings.h"

#include <libs/mediacenter/backendsmodel.h>
#include <libs/mediacenter/filterplaylistmodel.h>
#include <libs/mediacenter/filteredbackendsmodel.h>
#include <libs/mediacenter/subtitleprovider.h>
#include <libs/mediacenter/pmcimageprovider.h>
#include <libs/mediacenter/pmccoverartprovider.h>
#include <mediacenter/objectpair.h>
#include <mediacenter/filterplaylistmodel.h>
#include <mediacenter/multipleplaylistmodel.h>
#include <mpris2/mediaplayer2.h>
#include <mpris2/mediaplayer2player.h>
#include <mpris2/mpris2.h>

#include <Plasma/Package>
#include <Plasma/Theme>
#include <KDE/KCmdLineArgs>
#include <KDE/KApplication>

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QDeclarativeProperty>
#include <qdeclarative.h>

#include <QtOpenGL/QGLWidget>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(Application *parent)
    : KMainWindow(dynamic_cast <QWidget *> (parent))
    , m_mousePointerAutoHide(false)
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

    if (args->isSet("fullscreen") || Settings().value("fullscreen", false).toBool()) {
        toggleFullScreen();
    }

    view = new QDeclarativeView(this);

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

    view->setAttribute(Qt::WA_OpaquePaintEvent);
    view->setAttribute(Qt::WA_NoSystemBackground);
    view->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
    view->viewport()->setAttribute(Qt::WA_NoSystemBackground);

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    setCentralWidget(view);

    m_kdeclarative.setDeclarativeEngine(view->engine());
    m_kdeclarative.initialize();
    m_kdeclarative.setupBindings();

    qmlRegisterType<FilteredBackendsModel>("org.kde.plasma.mediacenter.elements", 0, 1, "FilteredBackendsModel");
    qmlRegisterType<SubtitleProvider>("org.kde.plasma.mediacenter.elements", 0, 1, "SubtitleProvider");
    qmlRegisterType<FilterPlaylistModel>("org.kde.plasma.mediacenter.elements", 0, 1, "FilterPlaylistModel");
    qmlRegisterType<MultiplePlaylistModel>("org.kde.plasma.mediacenter.elements", 0, 1, "MultiplePlaylistModel");
    qmlRegisterType<Settings>("org.kde.plasma.mediacenter.elements", 0, 1, "Settings");
    qmlRegisterType<MediaPlayer2>("org.kde.plasma.mediacenter.elements",0,1,"MediaPlayer2");
    qmlRegisterType<MediaPlayer2Player>("org.kde.plasma.mediacenter.elements",0,1,"MediaPlayer2Player");
    qmlRegisterInterface<ObjectPair>("ObjectPair");

    BackendsModel *backendsModel = new BackendsModel(view->engine(), this);
    view->rootContext()->setContextProperty("backendsModel", backendsModel);

    playlistModel = new PlaylistModel(this);
    if (playlistModel->processCommandLineArgs(args)) {
        QTimer::singleShot(500, this, SLOT(playPlaylist()));
    }
    view->rootContext()->setContextProperty("playlistModel", playlistModel);

    view->rootContext()->setContextProperty("_pmc_mainwindow", this);
    view->rootContext()->setContextProperty("startedFullScreen", isFullScreen());

    view->engine()->addImageProvider(PmcImageProvider::identificationString, new PmcImageProvider());
    view->engine()->addImageProvider(PmcCoverArtProvider::identificationString, new PmcCoverArtProvider());

    Plasma::Theme::defaultTheme()->setUseGlobalSettings(false);
    Plasma::Theme::defaultTheme()->setThemeName("oxygen");

    m_structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", m_structure);

    view->rootContext()->setContextProperty("_pmc_background_image_path", QUrl(package->filePath("images", "noiselight.png")));
    view->rootContext()->setContextProperty("_pmc_gradient_image_path", QUrl(package->filePath("images", "gradient.png")));
    view->rootContext()->setContextProperty("_pmc_shadow_image_path", QUrl(package->filePath("images", "shadow.png")));

    view->rootContext()->setContextProperty("_pmc_is_desktop", Settings().value("isDesktop",false));

    view->setSource(QUrl(package->filePath("mainscript")));

    resize(1366, 768);

    installEventFilter(this);
    centralWidget()->installEventFilter(this);
    
    Mpris2(this);

    m_mousePointerAutoHideTimer.setInterval(2000);
    connect(this, SIGNAL(mousePointerAutoHideChanged()), SLOT(enableMousePointerAutoHideIfNeeded()));
    connect(&m_mousePointerAutoHideTimer, SIGNAL(timeout()), SLOT(hideMousePointer()));
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

bool MainWindow::queryExit()
{
    Settings s;
    windowState() & Qt::WindowFullScreen ? s.setValue("fullscreen", true) : s.setValue("fullscreen", false);
    s.sync();
    return KMainWindow::queryExit();
}

void MainWindow::closeMediaCenter()
{
    QApplication::quit();
}

void MainWindow::hideMousePointer()
{
    kapp->setOverrideCursor(Qt::BlankCursor);
    m_mousePointerHidden = true;
}

void MainWindow::showMousePointer()
{
    kapp->setOverrideCursor(Qt::ArrowCursor);
    m_mousePointerHidden = false;
}

bool MainWindow::mousePointerAutoHide() const
{
    return m_mousePointerAutoHide;
}

void MainWindow::setMousePointerAutoHide(bool value)
{
    m_mousePointerAutoHide = value;
    emit mousePointerAutoHideChanged();
}

void MainWindow::enableMousePointerAutoHideIfNeeded()
{
    if (m_mousePointerAutoHide) {
        m_mousePointerAutoHideTimer.start();
    } else {
        m_mousePointerAutoHideTimer.stop();
        showMousePointer();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)
    if (event->type() == QEvent::HoverMove) {
        if (m_mousePointerHidden) {
            showMousePointer();
            enableMousePointerAutoHideIfNeeded();
        }
    }
    return false;
}

void MainWindow::playPlaylist()
{
    view->rootObject()->metaObject()->invokeMethod(view->rootObject(), "play");
}

void MainWindow::addNewInstanceArgsPlaylist()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (playlistModel->processCommandLineArgs(args)) {
        QTimer::singleShot(500, this, SLOT(playPlaylist()));
    }
}
