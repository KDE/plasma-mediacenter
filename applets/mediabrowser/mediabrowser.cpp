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
    
    qmlRegisterType<MediaCenter::AbstractBrowsingBackend>("MediaCenter", 0, 1, "AbstractBrowsingBackend");
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::showInstalledBackends()
{
    //TODO: always a view with all the avail backends in the scene instead
//    delete m_backend->model();
//    m_model = new StartupModel(this);
//    m_view->setModel(m_backend->model());

//    QmlHomeView *homeView = new QmlHomeView(this);
//    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
//    layout->addItem(homeView);

//    homeView->setQmlPath(KStandardDirs::locate("data", "plasma-mediacenter/declarative/homeview.qml"));
//    homeView->setModel(m_backend->model());
}

void MediaBrowser::init()
{
    m_view = new Plasma::DeclarativeWidget(this);
    m_layout->addItem(m_view);

    if (m_backend) {
        m_view->engine()->rootContext()->setContextProperty("fileBackend", m_backend);
    }
    m_view->setQmlPath(KStandardDirs::locate("data", "plasma-mediacenter/declarative/filegrid.qml"));

/* TODO:choose a backend by merging the welcome plasmoid*/
    KSharedPtr<KService>service = MediaCenter::AbstractBrowsingBackend::availableBackends().at(0);
    MediaCenter::AbstractBrowsingBackend *backend = service->createInstance<MediaCenter::AbstractBrowsingBackend>(0, QVariantList() << service->storageId());
    loadBrowsingBackend(backend);
    kDebug() << "backkkkkkkkkkkkeeeeeeeddddd" << m_backend->name();
}

void MediaBrowser::createConfigurationInterface(KConfigDialog *parent)
{
    if (currentBrowsingBackend()) {
        if (currentBrowsingBackend()->hasConfigurationInterface()) {
            currentBrowsingBackend()->createConfigurationInterface(parent);
        }
    }

    connect(parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}


void MediaBrowser::loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *backend)
{
    kDebug() << "loading browsing backend";
    
    m_backend = backend;

    m_backend->setParent(this);
    m_backend->init();
    m_view->engine()->rootContext()->setContextProperty("fileBackend", m_backend);

    emit browsingModeChanged( m_backend->requiredMode() );
}

void MediaBrowser::showStartupState()
{
}

KUrl MediaBrowser::currentUrl() const
{
    KDirModel *modeldir = qobject_cast<KDirModel*>(m_backend->model());
    if (!modeldir || !modeldir->dirLister()) {
        return KUrl();
    }
    return modeldir->dirLister()->url();
}

void MediaBrowser::openUrl(const KUrl &url)
{
    KDirModel *modeldir = qobject_cast<KDirModel*>(m_backend->model());
    if (modeldir) {
        modeldir->dirLister()->openUrl(url);
    }
}

void MediaBrowser::listMediaInDirectory()
{
    //m_view->listMediaInDirectory();
}

void MediaBrowser::selectedMediasAdd(const MediaCenter::Media &media)
{
    if (!m_selectedMedias.contains(media)) {
        m_selectedMedias << media;
        emit selectedMediasChanged(m_selectedMedias);
    }
}

void MediaBrowser::selectedMediasRemove(const MediaCenter::Media &media)
{
    if (m_selectedMedias.contains(media)) {
        m_selectedMedias.removeAll(media);
        emit selectedMediasChanged(m_selectedMedias);
    }
}

QList<MediaCenter::Media> MediaBrowser::selectedMedias() const
{
    return m_selectedMedias;
}

void MediaBrowser::clearSelectedMedias()
{
    m_selectedMedias.clear();
    emit selectedMediasChanged(m_selectedMedias);
}


K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
