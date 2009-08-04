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

#include <KDebug>
#include <KMenuBar>
#include <KMenu>
#include <KAction>
#include <KStandardAction>
#include <KIcon>
#include <KLocale>
#include <KPageDialog>
#include <KPluginInfo>

#include <QGraphicsView>
#include <QKeyEvent>

#include <Plasma/Corona>
#include <Plasma/Containment>
#include <Plasma/Theme>

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent),
m_view(new QGraphicsView(this)),
m_containment(0)
{
    Plasma::Theme::defaultTheme()->setUseGlobalSettings(false);

    setCentralWidget(m_view);

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
}

MainWindow::~MainWindow()
{}

void MainWindow::loadMediaCenter()
{
    m_containment = m_corona->addContainment("mediacontainment");
    if (!m_containment) {
        kDebug() << "unable to load mediacontaiment";
    }
    m_containment->addApplet("mediabrowser");
    m_containment->addApplet("playlist");
    m_containment->addApplet("mediacontroller");
    m_containment->addApplet("mcplayer");
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if (e->type() == QEvent::Resize && o == m_view) {
        if (m_containment) {
            m_containment->resize(size());
        }
    }

    if (e->type() == QEvent::KeyPress && o == this) {
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
    KPageDialog *dialog = new KPageDialog(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setCaption(i18n("Preferences"));

    QWidget *theme = new QWidget;
    m_theme.setupUi(theme);
    KPluginInfo::List themes = Plasma::Theme::listThemeInfo();
    foreach (const KPluginInfo &info, themes) {
        m_theme.themeComboBox->addItem(info.name());
    }

    kDebug() << Plasma::Theme::defaultTheme()->themeName();
    m_theme.themeComboBox->setCurrentIndex(m_theme.themeComboBox->findText(Plasma::Theme::defaultTheme()->themeName(), Qt::MatchContains));

    KPageWidgetItem *themeItem = dialog->addPage(theme, i18n("Theme settings"));
    themeItem->setIcon(KIcon("preferences-desktop-theme"));

    connect (dialog, SIGNAL(accepted()), this, SLOT(applyConfig()));
    dialog->exec();
}

void MainWindow::applyConfig()
{
    Plasma::Theme::defaultTheme()->setThemeName(m_theme.themeComboBox->currentText().toLower());
}
