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
#include "listview.h"
#include "gridview.h"
#include "abstractmediaitemview.h"

#include <mediacenter/abstractbrowsingbackend.h>
#include <mediacenter/browsergesture.h>
#include <mediacenter/widgets/navigationtoolbar.h>

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsLinearLayout>

#include <KDirModel>
#include <KDirLister>
#include <KIcon>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KFilePlacesModel>
#include <KUrl>
#include <KLineEdit>
#include <KMimeType>
#include <KStandardDirs>
#include <KDebug>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : MediaCenter::Browser(parent, args),
    m_view(0),
    m_model(0),
    m_layout(new QGraphicsLinearLayout(Qt::Vertical))
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setLayout(m_layout);
    setFiltersChildEvents(true);

    setEnableToolbar(true);
    toolbar()->setNavigationControls(MediaCenter::NavigationToolbar::BackwardControl | MediaCenter::NavigationToolbar::ViewModeControl);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::showInstalledBackends()
{
//    delete m_model;
//    m_model = new StartupModel(this);
//    m_view->setModel(m_model);

//    QmlHomeView *homeView = new QmlHomeView(this);
//    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
//    layout->addItem(homeView);

//    homeView->setQmlPath(KStandardDirs::locate("data", "plasma-mediacenter/declarative/homeview.qml"));
//    homeView->setModel(m_model);
}

void MediaBrowser::init()
{
    loadConfiguration();

    createView();
    showInstalledBackends();
}

void MediaBrowser::createView()
{
    delete m_view;
    kDebug() << "view deleted";

    if (m_viewType == "list") {
        m_view = new ListView(this);
    } else {
        m_view = new GridView(this);
    }

    m_view->setDrawBlurredText(m_blurred);
    m_view->setFocus(Qt::OtherFocusReason);

    if (m_model) {
        m_view->setModel(m_model);
//        m_view->generateItems();
    }

    connect (this, SIGNAL(browseHistoryBackRequest()), m_view, SLOT(goPrevious()));
    connect (m_view, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), this, SIGNAL(mediasActivated(QList<MediaCenter::Media>)));
    connect (m_view, SIGNAL(mediasListChanged(QList<MediaCenter::Media>)), this, SIGNAL(mediasListChanged(QList<MediaCenter::Media>)));
    connect (m_view, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SIGNAL(mediaActivated(const MediaCenter::Media&)));
    //connect (m_view, SIGNAL(indexActivated(QModelIndex)), this, SLOT(slotIndexActivated(QModelIndex)));
    connect (m_view, SIGNAL(mediaSelected(MediaCenter::Media)), this, SLOT(selectedMediasAdd(MediaCenter::Media)));
    connect (m_view, SIGNAL(mediaUnselected(MediaCenter::Media)), this, SLOT(selectedMediasRemove(MediaCenter::Media)));
    connect (m_view, SIGNAL(directoryChanged()), this, SLOT(clearSelectedMedias()));

    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
    layout->addItem(m_view);
}

void MediaBrowser::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *generalConfig = new QWidget(parent);
    uiGeneral.setupUi(generalConfig);

    parent->addPage(generalConfig, i18n("Browser appearance"), "preferences-desktop-display");
    if (m_viewType == "list") {
        uiGeneral.listRadio->setChecked(true);
    } else {
        uiGeneral.gridRadio->setChecked(true);
    }

    uiGeneral.blurredTextCheckBox->setChecked(m_blurred);

    if (currentBrowsingBackend()) {
        if (currentBrowsingBackend()->hasConfigurationInterface()) {
            currentBrowsingBackend()->createConfigurationInterface(parent);
        }
    }

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}

void MediaBrowser::loadConfiguration()
{
    KConfigGroup cf = config();

    m_viewType = cf.readEntry("ViewType", "list");
    m_blurred = cf.readEntry("BlurredText", true);
}

void MediaBrowser::configAccepted()
{
    KConfigGroup cf = config();
    kDebug() << cf.name();

    QString type;
    if (uiGeneral.listRadio->isChecked()) {
        type = "list";
    } else {
        type = "grid";
    }

    if (m_viewType != type) {
        m_viewType = type;
        createView();
    }

    cf.writeEntry("ViewType", type);
    bool blurred = uiGeneral.blurredTextCheckBox->isChecked();
    if (blurred != m_blurred) {
        m_blurred = blurred;
        m_view->setDrawBlurredText(m_blurred);
        cf.writeEntry("BlurredText", m_blurred);
    }

}

void MediaBrowser::loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *backend)
{
    m_view->setModel ( 0 );
    backend->init();
    m_model = backend->model();
    m_view->setModel ( m_model );

    emit browsingModeChanged ( backend->requiredMode() );
}

void MediaBrowser::showStartupState()
{
    //Calling directly showInstalledBackends makes PMC crash on startup
    //m_model = new StartupModel(this);
    //m_view->setModel(m_model);
}

KUrl MediaBrowser::currentUrl() const
{
    KDirModel *modeldir = qobject_cast<KDirModel*>(m_model);
    if (!modeldir || !modeldir->dirLister()) {
        return KUrl();
    }
    return modeldir->dirLister()->url();
}

void MediaBrowser::openUrl(const KUrl &url)
{
    KDirModel *modeldir = qobject_cast<KDirModel*>(m_model);
    if (modeldir) {
        modeldir->dirLister()->openUrl(url);
    }
}

void MediaBrowser::listMediaInDirectory()
{
    m_view->listMediaInDirectory();
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

void MediaBrowser::gestureEvent(MediaCenter::BrowserGesture *bGesture)
{
    if (!bGesture) {
        return;
    }

    if (bGesture->activeAction() == MediaCenter::BrowserGesture::BrowseLeftAction) {
        QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
        qApp->sendEvent(m_view, &keyEvent);
    } else if (bGesture->activeAction() == MediaCenter::BrowserGesture::BrowseRightAction) {
        QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
        qApp->sendEvent(m_view, &keyEvent);
    } else if (bGesture->activeAction() == MediaCenter::BrowserGesture::BrowseUpAction) {
        QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        qApp->sendEvent(m_view, &keyEvent);
    } else if (bGesture->activeAction() == MediaCenter::BrowserGesture::BrowseDownAction) {
        QKeyEvent keyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        qApp->sendEvent(m_view, &keyEvent);
    } else if (bGesture->activeAction() == MediaCenter::BrowserGesture::ActivateAction) {
        QKeyEvent keyEvent(QKeyEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        qApp->sendEvent(m_view, &keyEvent);
    }
}

void MediaBrowser::createQmlHomeView()
{}


K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
