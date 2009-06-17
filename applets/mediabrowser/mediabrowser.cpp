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
#include <KDebug>

#include <Nepomuk/ResourceManager>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : MediaCenter::Browser(parent, args),
    m_view(0),
    m_localUrl(KUrl()),
    m_fromPlaces(true),
    m_lister(0),
    m_model(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Nepomuk::ResourceManager::instance()->init();
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

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(m_view);
    setLayout(layout);
}

void MediaBrowser::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *generalConfig = new QWidget(parent);
    uiGeneral.setupUi(generalConfig);

    parent->addPage(generalConfig, i18n("Appearance"), "preferences-desktop-display");
    if (m_viewType == "list") {
        uiGeneral.listRadio->setChecked(true);
    } else {
        uiGeneral.gridRadio->setChecked(true);
    }

    QWidget *localConfig = new QWidget(parent);
    uiLocal.setupUi(localConfig);

    parent->addPage(localConfig, i18n("Local Browsing"), QString());

    KFilePlacesModel *model = new KFilePlacesModel(this);
    uiLocal.placesCombo->setModel(model);

    if (m_fromPlaces) {
        uiLocal.showPlace->setChecked(true);
        uiLocal.placesCombo->setCurrentIndex(model->closestItem(m_localUrl).row());
    } else {
        uiLocal.showCustomFolder->setChecked(true);
        uiLocal.urlRequester->lineEdit()->setText(m_localUrl.url());
    }

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

void MediaBrowser::loadConfiguration()
{
    KConfigGroup cf = config();

    m_localUrl = KUrl(cf.readEntry("LocalUrl", QDir::homePath()));
    m_fromPlaces = cf.readEntry("FromPlaces", true);
    m_viewType = cf.readEntry("ViewType", "list");
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

    cf.writeEntry("ViewType", type);

}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
