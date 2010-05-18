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
#include "localpicturespackage.h"

#include <mediabrowserlibs/mediabrowser_export.h>

#include <KConfigDialog>
#include <KDirModel>
#include <KDirLister>
#include <KMimeType>
#include <KFilePlacesModel>
#include <KLineEdit>
#include <KDebug>

MEDIABROWSER_PACKAGE_EXPORT(LocalPicturesPackage)

LocalPicturesPackage::LocalPicturesPackage(QObject *parent, const QVariantList &args) : ModelPackage(parent, args),
m_fromPlaces(true),
m_folderNavigation(true),
m_model(0)
{
    setHasConfigurationInterface(true);

    setAllowedMediaTypes(MediaCenter::Picture);
}

LocalPicturesPackage::~LocalPicturesPackage()
{}

QAbstractItemModel* LocalPicturesPackage::model()
{
    if (!m_model) {
        m_model = new KDirModel(this);

        KMimeType::List mimeList = KMimeType::allMimeTypes();

        foreach (KMimeType::Ptr mime, mimeList) {
            if (mime->name().startsWith("image/")) {
                m_mimeTypes << mime->name();
            }
        }
        setFolderNavigation();
        m_model->dirLister()->openUrl(m_localUrl);
    }

    return m_model;
}

ModelPackage::BrowsingType LocalPicturesPackage::browsingType() const
{
    return ModelPackage::LocalBrowsing;
}

void LocalPicturesPackage::init()
{
    KConfigGroup cf = config();

    m_localUrl = KUrl(cf.readEntry("LocalPicturesUrl", QDir::homePath()));
    m_fromPlaces = cf.readEntry("PicturesFromPlaces", true);
    m_folderNavigation = cf.readEntry("PicturesFolderNavigation", true);
}

void LocalPicturesPackage::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *localPicturesConfig = new QWidget(parent);
    uiLocal.setupUi(localPicturesConfig);

    parent->addPage(localPicturesConfig, i18n("Local Pictures Browsing"), "folder-development");

    KFilePlacesModel *model = new KFilePlacesModel(parent);
    uiLocal.placesCombo->setModel(model);

    if (m_fromPlaces) {
        uiLocal.showPlace->setChecked(true);
        uiLocal.placesCombo->setCurrentIndex(model->closestItem(m_localUrl).row());
    } else {
        uiLocal.showCustomFolder->setChecked(true);
        uiLocal.urlRequester->lineEdit()->setText(m_localUrl.url());
    }

    uiLocal.folderNavigationCheckBox->setChecked(m_folderNavigation);
    uiLocal.urlRequester->setMode(KFile::Directory);

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}

void LocalPicturesPackage::configAccepted()
{
    m_fromPlaces = uiLocal.showPlace->isChecked();
    m_localUrl = m_fromPlaces ? uiLocal.placesCombo->model()->index(uiLocal.placesCombo->currentIndex(), 0).data(KFilePlacesModel::UrlRole).value<QUrl>()
                              : uiLocal.urlRequester->url();

    qobject_cast<KDirLister*>(m_model->dirLister())->openUrl(m_localUrl);

    KConfigGroup cf = config();
    kDebug() << cf.name();

    cf.writeEntry("LocalPicturesUrl", m_localUrl);
    cf.writeEntry("PicturesFromPlaces", m_fromPlaces);

    bool folderNavigation = uiLocal.folderNavigationCheckBox->isChecked();
    if (m_folderNavigation != folderNavigation) {
        m_folderNavigation = folderNavigation;
        cf.writeEntry("PicturesFolderNavigation", m_folderNavigation);
        setFolderNavigation();
    }

    cf.sync();
}

void LocalPicturesPackage::setFolderNavigation()
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
    KDirLister *lister = qobject_cast<KDirLister*>(m_model->dirLister());
    if (lister) {
        lister->setMimeFilter(m_mimeTypes);
    }
}
