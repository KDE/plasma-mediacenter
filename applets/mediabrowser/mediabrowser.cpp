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
#include "mediabrowser.h"

#include <mediacenter/abstractbrowsingbackend.h>
#include <mediacenter/browsergesture.h>
#include <mediacenter/widgets/navigationtoolbar.h>

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsLinearLayout>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/qdeclarative.h>

#include <KDirModel>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KUrl>
#include <KStandardDirs>
#include <KDebug>
#include <KDirLister>
#include <KService>

QML_DECLARE_TYPE(MediaCenter::AbstractBrowsingBackend)

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : MediaCenter::Browser(parent, args),
    m_view(0),
    m_backend(0),
    m_layout(new QGraphicsLinearLayout(Qt::Vertical))
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setLayout(m_layout);

    setEnableToolbar(true);
    toolbar()->setNavigationControls(MediaCenter::NavigationToolbar::BackwardControl | MediaCenter::NavigationToolbar::ViewModeControl);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::init()
{
    m_view = new Plasma::DeclarativeWidget(this);
    m_layout->addItem(m_view);

    m_view->engine()->rootContext()->setContextProperty("mediaBrowserObject", this);
    m_view->setQmlPath(KStandardDirs::locate("data", "plasma-mediacenter/declarative/filegrid.qml"));

    MediaCenter::AbstractBrowsingBackend *backend;
    KSharedPtr<KService> service;

    KService::List list = MediaCenter::AbstractBrowsingBackend::availableBackends();

    for (int i=0; i<list.size(); ++i) {
        service = list.at(i);
        backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId());
        loadBrowsingBackend(backend);
    }
}

void MediaBrowser::loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *backend)
{
    if (!backend) {
        kDebug() << "OUCH! Something's wrong with the backend";
        return;
    }
    backend->setParent(this);
    m_backends.insert(backend->name(), backend);
}

QObject* MediaBrowser::backendFromName (const QString& backendName)
{
    return m_backends.value(backendName);
}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
