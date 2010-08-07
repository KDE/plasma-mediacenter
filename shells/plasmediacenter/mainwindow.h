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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KMainWindow>

#include "ui_theme.h"
#include "ui_backgrounddialog.h"


class QGraphicsView;
namespace Plasma {
    class Corona;
    class Containment;
    class Applet;
}

namespace Phonon {
    class VideoWidget;
}

namespace MediaCenter {
    class GestureRecognizer;
}

class KConfigSkeleton;

class MainWindow : public KMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool eventFilter(QObject *o, QEvent *e);

    void loadMediaCenter();

public slots:
    void createConfigurationInterface();

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *);

protected slots:
    void toggleFullScreen();
    void applyConfig();

private:
    KConfigGroup wallpaperConfig(Plasma::Containment * containment, const QString &plugin);
    QGraphicsView *m_view;
    Plasma::Corona *m_corona;
    Plasma::Containment *m_containment;
    KConfigSkeleton *m_cfskeleton;

    Plasma::Applet *m_browser;
    Plasma::Applet *m_controller;
    Plasma::Applet *m_playlist;
    Plasma::Applet *m_player;
    Plasma::Applet *m_infobar;
    Plasma::Applet *m_welcome;

    MediaCenter::GestureRecognizer *m_recognizer;
    Qt::GestureType m_gestureType;

    KUrl m_wallpaper;

    Ui::Theme m_theme;
    Ui::BackgroundDialog m_background;
};

#endif // MAINWINDOW_H
