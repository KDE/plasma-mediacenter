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
#include "localpicturesbackend.h"

#include <mediabrowserlibs/mediabrowser_export.h>

#include <KConfigDialog>
#include <KDirModel>
#include <KDirLister>
#include <KMimeType>
#include <KFilePlacesModel>
#include <KFileDialog>
#include <KLineEdit>
#include <KIcon>
#include <KDebug>

#include <Nepomuk/ResourceManager>
#include <Nepomuk/Variant>
#include <Nepomuk/Query/ResourceTypeTerm>
#include <Nepomuk/Query/QueryServiceClient>
#include <Nepomuk/Query/FileQuery>

#include "nfo.h"
#include "nie.h"

MEDIABROWSER_BACKEND_EXPORT(LocalPicturesBackend)

LocalPicturesBackend::LocalPicturesBackend(QObject *parent, const QVariantList &args) : AbstractBrowsingBackend(parent, args),
m_fromPlaces(true),
m_folderNavigation(true),
m_model(0)
{
    setHasConfigurationInterface(true);
    setAllowedMediaTypes(MediaCenter::Picture);

    Nepomuk::ResourceManager::instance()->init();
}

LocalPicturesBackend::~LocalPicturesBackend()
{}

QAbstractItemModel* LocalPicturesBackend::model()
{
    if (!m_model) {
        m_model = new KDirModel(this);
        initModel();
    }

    return m_model;
}

AbstractBrowsingBackend::BrowsingType LocalPicturesBackend::browsingType() const
{
    return AbstractBrowsingBackend::LocalBrowsing;
}

void LocalPicturesBackend::init()
{
    KConfigGroup cf = config();

    m_localUrl = KUrl(cf.readEntry("LocalPicturesUrl", QDir::homePath()));
    m_fromPlaces = cf.readEntry("PicturesFromPlaces", true);
    m_folderNavigation = cf.readEntry("PicturesFolderNavigation", true);
    m_useNepomuk = cf.readEntry("Nepomuk", true);
    m_watchedDirs = cf.readEntry("WatchedFolders", QStringList());
}

void LocalPicturesBackend::createConfigurationInterface(KConfigDialog *parent)
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
    connect(uiLocal.nepomukBox, SIGNAL(toggled(bool)), uiLocal.tab2, SLOT(setDisabled(bool)));
    connect(uiLocal.addFoldersButton, SIGNAL(clicked()), this, SLOT(getWatchedDirsList()));
    connect(uiLocal.removeFoldersButton, SIGNAL(clicked()), this, SLOT(removeItem()));

    uiLocal.addFoldersButton->setIcon(KIcon("list-add"));
    uiLocal.removeFoldersButton->setIcon(KIcon("list-remove"));

    uiLocal.nepomukBox->setChecked(m_useNepomuk);

    foreach(const QString &folder, m_watchedDirs) {
        uiLocal.watchedFoldersList->addItem(folder);
    }
}

void LocalPicturesBackend::getWatchedDirsList()
{
    KUrl url = KFileDialog::getExistingDirectoryUrl();
    for (int i = 0; i < uiLocal.watchedFoldersList->count(); i++) {
        if (uiLocal.watchedFoldersList->item(i)->text() == url.prettyUrl()) {
            return;
        }
    }
    uiLocal.watchedFoldersList->addItem(url.prettyUrl());
}

void LocalPicturesBackend::removeItem()
{
    qDeleteAll(uiLocal.watchedFoldersList->selectedItems());
}

void LocalPicturesBackend::configAccepted()
{
    KConfigGroup cf = config();

    bool useNepomuk = uiLocal.nepomukBox->isChecked();
    
    if (useNepomuk) {
        QStringList watchedDirs;
        for (int i = 0; i < uiLocal.watchedFoldersList->count(); i++) {
            watchedDirs << uiLocal.watchedFoldersList->item(i)->text();
        }
        m_watchedDirs = watchedDirs;

        cf.writeEntry("WatchedFolders", m_watchedDirs);
    } else {
        m_fromPlaces = uiLocal.showPlace->isChecked();
        m_localUrl = m_fromPlaces ? uiLocal.placesCombo->model()->index(uiLocal.placesCombo->currentIndex(), 0).data(KFilePlacesModel::UrlRole).value<QUrl>()
                                    : uiLocal.urlRequester->url();
        qobject_cast<KDirLister*>(m_model->dirLister())->openUrl(m_localUrl);

        cf.writeEntry("LocalPicturesUrl", m_localUrl);
        cf.writeEntry("PicturesFromPlaces", m_fromPlaces);

        bool folderNavigation = uiLocal.folderNavigationCheckBox->isChecked();
        if (m_folderNavigation != folderNavigation) {
            m_folderNavigation = folderNavigation;
            cf.writeEntry("PicturesFolderNavigation", m_folderNavigation);
            setFolderNavigation();
        }
    }

    if (useNepomuk != m_useNepomuk) {
        m_useNepomuk = useNepomuk;
        cf.writeEntry("Nepomuk", m_useNepomuk);
    }
    cf.sync();
    initModel();
}

void LocalPicturesBackend::initModel()
{
    if (m_useNepomuk) {
        m_model->dirLister()->setMimeFilter(QStringList());

        Nepomuk::Query::FileQuery query;
        query.setTerm(Nepomuk::Query::ResourceTypeTerm(Vocabulary::NFO::Image()));
        foreach (const QString &folder, m_watchedDirs) {
            query.addIncludeFolder(folder);
        }

        m_model->dirLister()->stop();
        m_model->dirLister()->openUrl(query.toSearchUrl());
    } else {
        KMimeType::List mimeList = KMimeType::allMimeTypes();

        foreach (KMimeType::Ptr mime, mimeList) {
            if (mime->name().startsWith("image/")) {
                m_mimeTypes << mime->name();
            }
        }
        setFolderNavigation();
        m_model->dirLister()->openUrl(m_localUrl);
    }
}

void LocalPicturesBackend::setFolderNavigation()
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
