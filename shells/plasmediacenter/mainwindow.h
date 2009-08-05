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

class QGraphicsView;
namespace Plasma {
    class Corona;
    class Containment;
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

protected slots:
    void toggleFullScreen();
    void applyConfig();

private:
    QGraphicsView *m_view;
    Plasma::Corona *m_corona;
    Plasma::Containment *m_containment;
    KConfigSkeleton *m_cfskeleton;

    Ui::Theme m_theme;
};

#endif // MAINWINDOW_H
