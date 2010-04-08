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
#include "startupmodel.h"

#include <mediabrowserlibs/modelpackage.h>

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
    m_model(0),
    m_browsingWidget(0),
    m_package(0),
    m_browsingWidgets(false)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    Nepomuk::ResourceManager::instance()->init();

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    if (m_browsingWidgets) {
        m_browsingWidget = new BrowsingWidget(this);
        layout->addItem(m_browsingWidget);
    }
    setLayout(layout);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::showInstalledModelPackages()
{
    delete m_model;
    m_model = new StartupModel(this);
    m_view->setModel(m_model);
}

void MediaBrowser::init()
{
    loadConfiguration();

    createView();
    showInstalledModelPackages();
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

    connect (m_browsingWidget, SIGNAL(goPrevious()), m_view, SLOT(goPrevious()));
    connect (m_view, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), this, SIGNAL(mediasActivated(QList<MediaCenter::Media>)));
    connect (m_view, SIGNAL(mediasListInDirectory(QList<MediaCenter::Media>)), this, SIGNAL(mediasListInDirectory(QList<MediaCenter::Media>)));
    connect (m_view, SIGNAL(mediaActivated(const MediaCenter::Media&)), this, SIGNAL(mediaActivated(const MediaCenter::Media&)));
    connect (m_view, SIGNAL(indexActivated(QModelIndex)), this, SLOT(slotIndexActivated(QModelIndex)));
    connect (this, SIGNAL(goPrevious()), m_view, SLOT(goPrevious()));
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

    uiGeneral.blurredTextCheckBox->setChecked(m_blurred);

    if (m_package) {
        if (m_package->hasConfigurationInterface()) {
            m_package->createConfigurationInterface(parent);
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

void MediaBrowser::slotIndexActivated(const QModelIndex &index)
{
    // let's see whether we are loadin' a plugin or not..
    StartupModel *model = qobject_cast<StartupModel*>(m_model);
    if (model) {
        QString error;
        ModelPackage *package = model->packageFromIndex(index, this, &error);
        if (!error.isEmpty()) {
            kError() << error;
            return;
        }
        m_view->setModel(0);
        m_package = package;
//        connect (package, SIGNAL(modelReady()), m_view, SLOT(generateItems()));
//        connect (package, SIGNAL(modelReady()), m_view, SLOT(updateScrollBar()));
        package->init();
        m_model = package->model();
        m_view->setModel(m_model);
    }
    kDebug() << "finished";
}

void MediaBrowser::setBrowsingWidgets(bool set)
{
    //TODO Implement some toggling of the browsing widgets, they will probably never be needed in the browser
}

bool MediaBrowser::browsingWidgets()
{
    return m_browsingWidgets;
}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
