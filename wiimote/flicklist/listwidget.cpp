/*
Copyright 2010 Sebastian Kügler <sebas@kde.org>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of
the License or (at your option) version 3 or any later version
accepted by the membership of KDE e.V. (or its successor approved
by the membership of KDE e.V.), which shall act as a proxy
defined in Section 14 of version 3 of the license.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// Own
#include "listwidget.h"

// Qt
#include <QGraphicsLinearLayout>

// KDE
#include <KDebug>
// Plasma
#include <Plasma/Label>
#include <Plasma/IconWidget>


ListWidget::ListWidget(QGraphicsWidget* parent) : Plasma::ScrollWidget(parent),
    m_layout(0)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_widget = new QGraphicsWidget(this);
    //m_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_layout = new QGraphicsLinearLayout(m_widget);
    m_layout->setOrientation(Qt::Vertical);
    //m_layout->setSpacing(1);
    setWidget(m_widget);
    setMinimumSize(100, 100);
    setPreferredSize(200, 200);
    velocity = 20;
    //m_widget->setMinimumSize(100, 100);
    //m_widget->setPreferredSize(200, 200);
    //QStringList knoppen;
    //knoppen << "eins" << "zwei" << "drei" << "vier" << "fünf" << "sechs";
    //knoppen << knoppen;
    //knoppen << knoppen;
    //Plasma::Label* lbl = new Plasma::Label(m_widget);
    //lbl->setText("<h3>boven!</h3>");
    //m_layout->addItem(lbl);

    m_dirLister = new KDirLister(this);
    connect(m_dirLister, SIGNAL(itemsAdded(const KUrl &, const KFileItemList &)),
            this, SLOT(itemsAdded(const KUrl &, const KFileItemList &)));
    /*
    foreach (const QString &txt, knoppen) {
        Plasma::IconWidget* knop = new Plasma::IconWidget(m_widget);
        knop->setIcon("image");
        knop->setOrientation(Qt::Horizontal);
        knop->setMinimumHeight(48);
        knop->setText(txt);
        m_layout->addItem(knop);
        //kDebug() << "new item: " << txt;
    }
    */
    listDir(KUrl("/data/Music/"));

    m_wiimote = new Wiimote(this);
    connect(m_wiimote, SIGNAL(dragUpDown(int)), this, SLOT(wiimoteDragUpDown(int)));
    connect(m_wiimote, SIGNAL(flickUpDown(int)), this, SLOT(wiimoteFlickUpDown(int)));
    m_wiimote->start();
}

void ListWidget::wiimoteFlickUpDown(int moves)
{
    qreal velo = moves * velocity;
    QPointF p = QPointF(0, qMin(velo, 500.0));
    if (!p.isNull()) {
        //kDebug() << "list flicking to" << p;
        //kDebug() << "needs patch to kdelibs, then enable flick(p); in listwidget.cpp";
        //flick(p);
    }
}

void ListWidget::wiimoteDragUpDown(int moves)
{
    //if (moves < 0) return;
    int m = 1; // multiplyer movement / pixels
    QPointF p = scrollPosition();
    //kDebug() << scrollPosition();
    qreal ynew = qMax((qreal)(0.0), (-moves * m) + p.y());
    //kDebug() << "Preferred height, ynew:" << m_widget->preferredHeight() << ynew;
    ynew = qMin(m_widget->preferredHeight(), ynew);
    QPointF p2 = QPointF(p.x(), ynew);
    setScrollPosition(p2);
    //kDebug() << "scrolled" << p << " to" << p2 << moves;
}


void ListWidget::listDir(const KUrl &url)
{
    m_dirLister->openUrl(url);
}

Wiimote* ListWidget::wiimote()
{
    return m_wiimote;
}

void ListWidget::itemsAdded (const KUrl &directoryUrl, const KFileItemList &items)
{
    foreach (KFileItem item, items) {

        Plasma::IconWidget* knop = new Plasma::IconWidget(m_widget);
        knop->setOrientation(Qt::Horizontal);
        knop->setMinimumHeight(48);
        knop->setMaximumWidth(400);
        //knop->setText(txt);
        m_layout->addItem(knop);

        // Set file info
        knop->setIcon(item.iconName());
        knop->setText(item.name());
        knop->setInfoText(item.mimetype());
    }
    kDebug() << "Preferred height:" << m_widget->preferredHeight();
}

ListWidget::~ListWidget()
{
}


// vim: sw=4 sts=4 et tw=100
