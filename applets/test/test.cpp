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
#include "test.h"
#include "widgets/activeitemwidget.h"

#include <mediacenter/mediacenter.h>

#include <KDebug>

#include <QWidget>
#include <QCheckBox>
#include <QGraphicsLinearLayout>
#include <QGraphicsProxyWidget>

#include <Plasma/LineEdit>

MCTest::MCTest(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{

    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    ActiveItemWidget *widget = new ActiveItemWidget;
    widget->addItem("Item 1");
    widget->addItem("Item 2");
    layout->addItem(widget);

    m_edit = new Plasma::LineEdit(this);
    layout->addItem(m_edit);
    connect (m_edit, SIGNAL(returnPressed()), this, SLOT(doDebug()));

    QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(this);
    QWidget *w = new QWidget;
    QCheckBox *cbox = new QCheckBox("helo", w);
    QPalette p = w->palette();
    p.setColor(QPalette::Window, Qt::green);
    w->setPalette(p);
    w->setMinimumHeight(5);
    proxy->setWidget(w);
    layout->addItem(proxy);

    setLayout(layout);

}

MCTest::~MCTest()
{}

void MCTest::init()
{
}

void MCTest::doDebug()
{
    kDebug() << (int)MediaCenter::getType(m_edit->text());
}

K_EXPORT_PLASMA_APPLET(mctest, MCTest)
