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
#include "browsingwidget.h"

#include <QWidget>
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
#include <KDebug>

#include <Nepomuk/ResourceManager>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : MediaCenter::Browser(parent, args),
    m_view(0),
    m_localUrl(KUrl()),
    m_fromPlaces(true),
    m_lister(0),
    m_model(0),
    m_browsingWidget(new BrowsingWidget(this))
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Nepomuk::ResourceManager::instance()->init();

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    layout->addItem(m_browsingWidget);
    setLayout(layout);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::init()
{
    loadConfiguration();

    createView();
    switchToFileModel();
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
//    m_view->setScrollMode(AbstractMediaItemView::PerItem);

    connect (m_browsingWidget, SIGNAL(goPrevious()), m_view, SLOT(goPrevious()));
    QGraphicsLinearLayout *layout = static_cast<QGraphicsLinearLayout*>(this->layout());
    layout->addItem(m_view);
    setLayout(layout);
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

    QWidget *localConfig = new QWidget(parent);
    uiLocal.setupUi(localConfig);

    parent->addPage(localConfig, i18n("Local Browsing"), "folder-development");

    KFilePlacesModel *model = new KFilePlacesModel(this);
    uiLocal.placesCombo->setModel(model);

    if (m_fromPlaces) {
        uiLocal.showPlace->setChecked(true);
        uiLocal.placesCombo->setCurrentIndex(model->closestItem(m_localUrl).row());
    } else {
        uiLocal.showCustomFolder->setChecked(true);
        uiLocal.urlRequester->lineEdit()->setText(m_localUrl.url());
    }

    uiGeneral.folderNavigationCheckBox->setChecked(m_folderNavigation);
    uiGeneral.blurredTextCheckBox->setChecked(m_blurred);

    uiLocal.urlRequester->setMode(KFile::Directory);

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}

void MediaBrowser::switchToFileModel()
{
    kDebug() << "";
    if (!m_model) {
        m_model = new KDirModel(this);
    }

   if (!m_lister) {
        m_lister = new KDirLister(this);
        KMimeType::List mimeList = KMimeType::allMimeTypes();

        foreach (KMimeType::Ptr mime, mimeList) {
            if (mime->name().startsWith("image/") ||
                mime->name().startsWith("video/") ||
                mime->name().startsWith("audio/")) {
                m_mimeTypes << mime->name();
            }
        }
        setFolderNavigation();
    }

    if (m_model->dirLister() != m_lister) {
        m_model->setDirLister(m_lister);
    }

    connect (m_lister, SIGNAL(completed()), m_view, SLOT(updateScrollBar()));
    connect (m_lister, SIGNAL(completed()), m_view, SLOT(generateItems()));

    m_lister->openUrl(m_localUrl);

    m_view->setModel(m_model);

    m_mode = LocalFiles;
}

void MediaBrowser::setFolderNavigation()
{
    if (m_folderNavigation) {
        if (!m_mimeTypes.contains("inode/directory")) {
            m_mimeTypes << "inode/directory";
        }
    } else {
        if (m_mimeTypes.contains("inode/directory")) {
            m_mimeTypes.removeAll("inode/directory");
        }
    }
    m_lister->setMimeFilter(m_mimeTypes);
}

void MediaBrowser::loadConfiguration()
{
    KConfigGroup cf = config();

    m_localUrl = KUrl(cf.readEntry("LocalUrl", QDir::homePath()));
    m_fromPlaces = cf.readEntry("FromPlaces", true);
    m_viewType = cf.readEntry("ViewType", "list");
    m_folderNavigation = cf.readEntry("FolderNavigation", true);
    m_blurred = cf.readEntry("BlurredText", true);
}

void MediaBrowser::configAccepted()
{
    m_fromPlaces = uiLocal.showPlace->isChecked();
    m_localUrl = m_fromPlaces ? uiLocal.placesCombo->model()->index(uiLocal.placesCombo->currentIndex(), 0).data(KFilePlacesModel::UrlRole).value<QUrl>()
                              : uiLocal.urlRequester->url();

    if (m_mode == LocalFiles) {
        m_lister->openUrl(m_localUrl);
    }

    KConfigGroup cf = config();
    kDebug() << cf.name();

    cf.writeEntry("LocalUrl", m_localUrl);
    cf.writeEntry("FromPlaces", m_fromPlaces);

    QString type;
    if (uiGeneral.listRadio->isChecked()) {
        type = "list";
    } else {
        type = "grid";
    }

    if (m_viewType != type) {
        m_viewType = type;
        createView();
        switchToFileModel();
    }

    bool folderNavigation = uiGeneral.folderNavigationCheckBox->isChecked();
    if (m_folderNavigation != folderNavigation) {
        m_folderNavigation = folderNavigation;
        cf.writeEntry("FolderNavigation", m_folderNavigation);
        setFolderNavigation();
    }

    cf.writeEntry("ViewType", type);
    bool blurred = uiGeneral.blurredTextCheckBox->isChecked();
    if (blurred != m_blurred) {
        m_blurred = blurred;
        m_view->setDrawBlurredText(m_blurred);
        cf.writeEntry("BlurredText", m_blurred);
    }

}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
