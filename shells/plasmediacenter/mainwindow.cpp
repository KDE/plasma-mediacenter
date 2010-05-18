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

#include <QGraphicsView>
#include <QKeyEvent>
#include <QWheelEvent>

#include <KAction>
#include <KCmdLineArgs>
#include <KConfigDialog>
#include <KConfigSkeleton>
#include <KDebug>
#include <KIcon>
#include <KLocale>
#include <KMenu>
#include <KMenuBar>
#include <KPluginInfo>
#include <KSharedConfig>
#include <KSharedPtr>
#include <KStandardAction>

#include <Plasma/Corona>
#include <Plasma/Containment>
#include <Plasma/Theme>
#include <Plasma/Applet>
#include <Plasma/Wallpaper>

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent),
m_view(new QGraphicsView(this)),
m_containment(0),
m_cfskeleton(new KConfigSkeleton),
m_browser(0),
m_controller(0),
m_playlist(0),
m_player(0),
m_infobar(0)
{
    setCentralWidget(m_view);

    m_view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_corona = new Plasma::Corona(this);
    m_view->setScene(m_corona);

    m_view->installEventFilter(this);
    installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShadow(QFrame::Plain);
    m_view->setFrameShape(QFrame::NoFrame);

    KAction *fullScreen = new KAction(i18n("Go fullscreen"), this);
    fullScreen->setShortcut(Qt::CTRL + Qt::Key_F);
    fullScreen->setIcon(KIcon("view-fullscreen"));
    connect (fullScreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));


    KAction *preferences = KStandardAction::preferences(this, SLOT(createConfigurationInterface()), this);

    QMenu *menu = menuBar()->addMenu(i18n("Settings"));
    menu->addAction(fullScreen);
    menu->addSeparator();
    menu->addAction(preferences);

    menuBar()->addMenu(helpMenu());

    resize(1024, 600); //netbook size
    //showFullScreen();

    KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
    if (args->isSet("fullscreen")) {
        toggleFullScreen();
    }
}

MainWindow::~MainWindow()
{}

void MainWindow::loadMediaCenter()
{
    m_containment = m_corona->addContainment("mediacontainment");
    if (!m_containment) {
        kDebug() << "unable to load mediacontaiment";
        return;
    }

    m_containment->resize(m_view->size());
    m_view->setSceneRect(m_containment->geometry());

    m_browser = m_containment->addApplet("mediabrowser");
    m_playlist = m_containment->addApplet("playlist");
    m_player = m_containment->addApplet("mcplayer");
    m_infobar = m_containment->addApplet("mediainfobar");
    m_controller = m_containment->addApplet("mediacontroller"); //Keep the controller last
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    Q_UNUSED(o);

    if (e->type() == QEvent::KeyPress) {
        QKeyEvent *key = static_cast<QKeyEvent*>(e);
        if (key->key() == Qt::Key_Escape && windowState() & Qt::WindowFullScreen) {
            toggleFullScreen();
        }
    }

    return false;
}

void MainWindow::toggleFullScreen()
{
    if (windowState() & Qt::WindowFullScreen) {
        setWindowState(windowState() & ~Qt::WindowFullScreen);
        menuBar()->show();
    } else {
        setWindowState(windowState() | Qt::WindowFullScreen);
        menuBar()->hide();
    }
}

void MainWindow::createConfigurationInterface()
{
    KConfigDialog *dialog = new KConfigDialog(this, "Settings", m_cfskeleton);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setCaption(i18n("Preferences"));

    //The settings
    QWidget *theme = new QWidget;
    m_theme.setupUi(theme);
    KPluginInfo::List themes = Plasma::Theme::listThemeInfo();
    foreach (const KPluginInfo &info, themes) {
        m_theme.themeComboBox->addItem(info.name(), info.pluginName());
    }

    kDebug() << Plasma::Theme::defaultTheme()->themeName();
    for (int i = 0; i < m_theme.themeComboBox->count(); i++) {
        if (m_theme.themeComboBox->itemData(i).toString() == Plasma::Theme::defaultTheme()->themeName()) {
            m_theme.themeComboBox->setCurrentIndex(i);
            break;
        }
    }
    //Background Settings
    QWidget *background = new QWidget;
    m_background.setupUi(background);



    KPageWidgetItem *themeItem = dialog->addPage(theme, i18n("Theme settings"));
    themeItem->setIcon(KIcon("preferences-desktop-theme"));
    KPageWidgetItem *backgroundItem = dialog->addPage(background, i18n("Background settings"));
    backgroundItem->setIcon(KIcon("preferences-desktop-theme"));

    if (m_browser) {
        m_browser->createConfigurationInterface(dialog);
    }
    if (m_controller) {
        m_controller->createConfigurationInterface(dialog);
    }
    if (m_playlist) {
        m_playlist->createConfigurationInterface(dialog);
    }
    if (m_player) {
        m_player->createConfigurationInterface(dialog);
    }
    if (m_infobar) {
        m_infobar->createConfigurationInterface(dialog);
    }

    connect (dialog, SIGNAL(accepted()), this, SLOT(applyConfig()));
    dialog->exec();
}

void MainWindow::applyConfig()
{
    kDebug() << m_theme.themeComboBox->itemData(m_theme.themeComboBox->currentIndex()).toString();
    Plasma::Theme::defaultTheme()->setThemeName(m_theme.themeComboBox->itemData(m_theme.themeComboBox->currentIndex()).toString());
    kWarning() << m_background.fileRequester->url();
    m_wallpaper = m_background.fileRequester->url().toLocalFile();

    Plasma::Wallpaper *currentWallpaper = m_containment->wallpaper();
    if (currentWallpaper) {
        KConfigGroup cfg = wallpaperConfig(m_containment, currentWallpaper->pluginName());
        currentWallpaper->save(cfg);
    }
    KConfigGroup cfg = wallpaperConfig(m_containment, "image");
    cfg.writeEntry("wallpaper", m_wallpaper);
    //cfg.writeEntry("wallpapercolor", (color.isEmpty() ? "0,0,0" : color));
    //cfg.writeEntry("wallpaperposition", position);
    cfg.sync();
    m_containment->setWallpaper("image", "SingleImage");
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (!m_containment) {
        return;
    }
    m_containment->resize(m_view->size());
    m_view->setSceneRect(m_containment->geometry());
}

KConfigGroup MainWindow::wallpaperConfig(Plasma::Containment * containment, const QString &plugin)
{
    Q_ASSERT(containment);

    //FIXME: we have details about the structure of the containment config duplicated here!

    KConfigGroup cfg = containment->config();
    cfg = KConfigGroup(&cfg, "Wallpaper");
    return KConfigGroup(&cfg, plugin);
}


