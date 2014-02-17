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

#include <Plasma/PackageStructure>
#include <kdeclarative.h>

#include <QDeclarativeView>
#include <QtCore/QTimer>
#include <libs/mediacenter/playlistmodel.h>
#include "application.h"

class QKeyEvent;
class QMouseEvent;

class MainWindow : public KMainWindow
{
    Q_OBJECT
    Q_PROPERTY(bool mousePointerAutoHide READ mousePointerAutoHide
                WRITE setMousePointerAutoHide NOTIFY mousePointerAutoHideChanged)
public:
    explicit MainWindow(Application *parent);

    bool mousePointerAutoHide() const;
    void setMousePointerAutoHide(bool value);

public Q_SLOTS:
    void closeMediaCenter();
    bool toggleFullScreen();
    void hideMousePointer();
    void showMousePointer();
    void playPlaylist();
    void addNewInstanceArgsPlaylist();

Q_SIGNALS:
    void mousePointerAutoHideChanged();

private Q_SLOTS:
    void enableMousePointerAutoHideIfNeeded();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    virtual bool queryExit();

private:
    Plasma::PackageStructure::Ptr m_structure;
    KDeclarative m_kdeclarative;

    bool m_mousePointerAutoHide;
    bool m_mousePointerHidden;
    QTimer m_mousePointerAutoHideTimer;

    QDeclarativeView *view;
    PlaylistModel *playlistModel;
};

#endif // MAINWINDOW_H
