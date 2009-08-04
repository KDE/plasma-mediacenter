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

#include <Plasma/Corona>
#include <Plasma/Containment>

MainWindow::MainWindow(QWidget *parent) : KMainWindow(parent),
m_view(new QGraphicsView(this)),
m_containment(0)
{
    setCentralWidget(m_view);

    m_corona = new Plasma::Corona(this);
    m_view->setScene(m_corona);

    m_view->installEventFilter(this);

    m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_view->setFrameShadow(QFrame::Plain);
    m_view->setFrameShape(QFrame::NoFrame);
}

MainWindow::~MainWindow()
{}

void MainWindow::loadMediaCenter()
{
    m_containment = m_corona->addContainment("mediacontainment");
}

bool MainWindow::eventFilter(QObject *o, QEvent *e)
{
    if (o != m_view) {
        return false;
    }

    if (e->type() == QEvent::Resize) {
        if (m_containment) {
            m_containment->resize(size());
        }
    }
    return false;
}
