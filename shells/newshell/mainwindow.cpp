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

#include <config-plasma-mediacenter.h>

#include <backendsmodel.h>
#include <filterplaylistmodel.h>
#include <filteredbackendsmodel.h>
#include <subtitleprovider.h>
#include <pmccoverartprovider.h>
#include <objectpair.h>
#include <settings.h>
#include <multipleplaylistmodel.h>
#include <mpris2/mediaplayer2player.h>
#include <mpris2/mpris2.h>
#include <pmcruntime.h>

#ifndef NO_LINK_TO_PLASMA
#include <Plasma/Package>
#include <Plasma/Theme>
#endif

#include <KDE/KCmdLineArgs>
#include <KDE/KApplication>
#include <KDE/KWindowSystem>

#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeContext>
#include <QDeclarativeProperty>
#include <qdeclarative.h>

#include <QLibrary>
#include <QtOpenGL/QGLWidget>
#include <QtGui/QApplication>
#include <QtGui/QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QDir>

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
    qmlRegisterInterface<ObjectPair>("ObjectPair");

    playlistModel = QSharedPointer<PlaylistModel>(new PlaylistModel(this));
    playlistModel->processCommandLineArgs(args);
    view->rootContext()->setContextProperty("playlistModel", playlistModel.data());

    BackendsModel *backendsModel = new BackendsModel(createPmcRuntime(view->engine(), playlistModel), this);
    view->rootContext()->setContextProperty("backendsModel", backendsModel);

    Mpris2 *mprisObject = new Mpris2(playlistModel, this);
    connect(mprisObject, SIGNAL(raisePMC()), this, SLOT(raiseAndFocusPMC()));

    MediaPlayer2Player *mp2p = mprisObject->getMediaPlayer2Player();
    view->rootContext()->setContextProperty("mprisPlayerObject", mp2p);

    view->rootContext()->setContextProperty("_pmc_mainwindow", this);
    view->rootContext()->setContextProperty("startedFullScreen", isFullScreen());

    view->engine()->addImageProvider(PmcCoverArtProvider::identificationString, new PmcCoverArtProvider());

    QString imagesPath;
    QString mainscriptPath;

    if (!loadThemeAndPopulateMainscriptAndImagesPath(mainscriptPath, imagesPath)) {
        qFatal("Failed to load Plasma Libraries, cannot continue.");
    }

    view->rootContext()->setContextProperty("_pmc_background_image_path", QUrl(QDir(imagesPath).absoluteFilePath("noiselight.png")));
    view->rootContext()->setContextProperty("_pmc_gradient_image_path", QUrl(QDir(imagesPath).absoluteFilePath("gradient.png")));
    view->rootContext()->setContextProperty("_pmc_shadow_image_path", QUrl(QDir(imagesPath).absoluteFilePath("shadow.png")));

    view->rootContext()->setContextProperty("_pmc_is_desktop", Settings().value("isDesktop",false));

    view->setSource(QUrl(mainscriptPath));

    resize(1366, 768);

    installEventFilter(this);
    centralWidget()->installEventFilter(this);

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    Settings s;
    s.setValue("fullscreen", (windowState() & Qt::WindowFullScreen) != 0);
    s.sync();
    KMainWindow::closeEvent(event);
}

void MainWindow::closeMediaCenter()
{
    QCloseEvent event;
    closeEvent(&event);
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

void MainWindow::addNewInstanceArgsPlaylist()
{
    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    playlistModel->processCommandLineArgs(args);
}

bool MainWindow::loadThemeAndPopulateMainscriptAndImagesPath(QString& mainscriptPath, QString& imagesPath)
{
#ifdef NO_LINK_TO_PLASMA
    qDebug() << "PLASMA_ADAPTER: Setting Oxygen Plasma theme and loading resources...";
    typedef void (*MyPrototype)();
    typedef QString (*QStringPrototype)();

    QLibrary plasmaAdapterLib(PLASMAADAPTERLIBNAME);
    MyPrototype loadPlasmaTheme = (MyPrototype) plasmaAdapterLib.resolve("loadPlasmaTheme");
    QStringPrototype getMainscriptPath = (QStringPrototype) plasmaAdapterLib.resolve("getMainscriptPath");
    QStringPrototype getImagesPath = (QStringPrototype) plasmaAdapterLib.resolve("getImagesPath");

    if (!plasmaAdapterLib.isLoaded()) {
        qDebug() << plasmaAdapterLib.errorString();
        return false;
    }

    if (loadPlasmaTheme) {
        loadPlasmaTheme();
    }

    if (getMainscriptPath) {
        mainscriptPath = getMainscriptPath();
    }

    if (getImagesPath) {
        imagesPath = getImagesPath();
    }

    plasmaAdapterLib.unload();

#else
    qDebug() << "Setting Oxygen Plasma theme and loading resources...";
    Plasma::Theme::defaultTheme()->setUseGlobalSettings(false);
    Plasma::Theme::defaultTheme()->setThemeName("oxygen");

    Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
    Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", structure);

    mainscriptPath = package->filePath("mainscript");

    imagesPath = package->filePath("images");
#endif
    return true;
}

void MainWindow::raiseAndFocusPMC()
{
    raise();
    KWindowSystem::forceActiveWindow(winId());
}

QSharedPointer<PmcRuntime> MainWindow::createPmcRuntime(QDeclarativeEngine* engine,
                                         const QSharedPointer<PlaylistModel> &playlistModel)
{
    QHash< PmcRuntime::RuntimeObjectType, QSharedPointer< QObject > > runtimeObjects;
    runtimeObjects.insert(PmcRuntime::PlaylistModel, playlistModel);

    return QSharedPointer<PmcRuntime>(new PmcRuntime(runtimeObjects, engine, this));
}
