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

#include <QWidget>
#include <QGraphicsLinearLayout>

#include <KDirModel>
#include <KDirLister>
#include <KIcon>
#include <KConfigDialog>
#include <KConfigGroup>
#include <KFilePlacesModel>
#include <KUrl>
#include <KDebug>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args),
    m_listView(new ListView(this)),
    m_localUrl(KUrl()),
    m_fromPlaces(true)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(m_listView);
    setLayout(layout);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::init()
{
    loadConfiguration();
    switchToFileModel();
}

void MediaBrowser::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *generalConfig = new QWidget(parent);
    uiGeneral.setupUi(generalConfig);

    parent->addPage(generalConfig, i18n("Appearance"), "preferences-desktop-display");


    QWidget *localConfig = new QWidget(parent);
    uiLocal.setupUi(localConfig);

    parent->addPage(localConfig, i18n("Local Browsing"), QString());

    KFilePlacesModel *model = new KFilePlacesModel(this);
    uiLocal.placesCombo->setModel(model);

    if (m_fromPlaces) {
        uiLocal.showPlace->setChecked(true);
    } else {
        uiLocal.showCustomFolder->setChecked(true);
    }

    uiLocal.urlRequester->setMode(KFile::Directory);
    uiLocal.placesCombo->setCurrentIndex(model->closestItem(m_localUrl).row());

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}

void MediaBrowser::switchToFileModel()
{
    KDirModel *model = new KDirModel(this);

    m_lister = new KDirLister(this);
    // TODO: always use the current view
    connect (m_lister, SIGNAL(completed()), m_listView, SLOT(updateScrollBar()));
    connect (m_lister, SIGNAL(completed()), m_listView, SLOT(generateItems()));

    model->setDirLister(m_lister);
    m_lister->openUrl(m_localUrl);
    m_listView->setModel(model);

    m_mode = LocalFiles;
}

void MediaBrowser::loadConfiguration()
{
    KConfigGroup cf = config();

    m_localUrl = KUrl(cf.readEntry("LocalUrl", QDir::homePath()));
    m_fromPlaces = cf.readEntry("FromPlaces", true);
}

void MediaBrowser::configAccepted()
{
    m_fromPlaces = uiLocal.showPlace->isChecked();
    m_localUrl = m_fromPlaces ? uiLocal.placesCombo->model()->index(uiLocal.placesCombo->currentIndex(), 0).data(KFilePlacesModel::UrlRole).value<QUrl>()
                              : uiLocal.urlRequester->url();

    KConfigGroup cf = config();
    kDebug() << cf.name();

    cf.writeEntry("LocalUrl", m_localUrl);
    cf.writeEntry("FromPlaces", m_fromPlaces);

    // update current browsing mode according to the options
    if (m_mode == LocalFiles) {
        m_listView->invalidate();
        m_lister->openUrl(m_localUrl);
    }
}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
