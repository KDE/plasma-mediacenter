/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#include "mediawelcome.h"

#include "qmlviewwrapper.h"
#include "backendmodel.h"

#include <mediacenter/abstractbrowsingbackend.h>

#include <QGraphicsLinearLayout>
#include <QDeclarativeItem>

#include <KStandardDirs>
#include <KService>
#include <KDebug>
#include <plasma/widgets/declarativewidget.h>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

MediaWelcome::MediaWelcome(QObject *parent, const QVariantList &args) : MediaCenter::HomeApplet(parent, args),
    m_model(0),
    m_view(0)
{
    setAcceptedMouseButtons(Qt::NoButton);
}

MediaWelcome::~MediaWelcome()
{}

void MediaWelcome::init()
{
    m_model = new BackendModel(this);
    m_model->setModelServices(MediaCenter::AbstractBrowsingBackend::availableBackends());

    Plasma::DeclarativeWidget *qmlWidget = new Plasma::DeclarativeWidget(this);
    qmlWidget->engine()->rootContext()->setContextProperty("homeModel", m_model);
    qmlWidget->setQmlPath(KStandardDirs::locate("data", "plasma-mediacenter/declarative/homeview.qml"));
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(qmlWidget);
    setLayout(layout);
    kDebug() << m_model->rowCount();

    /*QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(m_view->rootObject());
    connect(item, SIGNAL(clicked()), this, SLOT(itemActivated()));*/
}

void MediaWelcome::itemActivated()
{
    QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(m_view->rootObject());
    KService *service = m_model->index(item->property("currentIndex").toInt(), 0).data(BackendModel::ServiceRole).value<KService*>();
    MediaCenter::AbstractBrowsingBackend *backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId());

    kDebug() << "browsing backend chosen";
    emit browsingBackendActivated(backend);
}

K_EXPORT_PLASMA_APPLET(mediawelcome, MediaWelcome)
