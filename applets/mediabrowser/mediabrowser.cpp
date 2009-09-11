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
#include <KService>
#include <KServiceTypeTrader>
#include <KDebug>

#include <Nepomuk/ResourceManager>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : MediaCenter::Browser(parent, args),
    m_view(0),
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

void MediaBrowser::showInstalledModelPackages()
{
    KService::List plugins = KServiceTypeTrader::self()->query("Plasma/MediaCenter/ModelPackage");
    if (plugins.isEmpty()) {
        kDebug() << "no available model package";
        return;
    }
    foreach (const KSharedPtr<KService> service, plugins) {
        kDebug() << "Name:" << service->name() << "Comment:" << service->comment();
    }

    StartupModel *model = new StartupModel(this);
    model->addAvailableModels(plugins);
    m_view->setModel(model);
    m_view->generateItems();
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
//    m_view->setScrollMode(AbstractMediaItemView::PerItem);

    connect (m_browsingWidget, SIGNAL(goPrevious()), m_view, SLOT(goPrevious()));
    connect (m_view, SIGNAL(mediasActivated(QList<MediaCenter::Media>)), this, SIGNAL(mediasActivated(QList<MediaCenter::Media>)));
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

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
