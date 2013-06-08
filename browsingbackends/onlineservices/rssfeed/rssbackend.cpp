/*
 * Copyright 2013 Gregor Taetzner gregor@freenet.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rssbackend.h"

#include "rssmanager.h"
#include "proxymodel.h"


MEDIACENTER_EXPORT_BROWSINGBACKEND(RssBackend)

RssBackend::RssBackend(QObject *parent, const QVariantList &args):
	AbstractBrowsingBackend ( parent, args ),
	m_rssmanager(0)
{
	m_rsspanelqml =  constructQmlSource("rsscomponents", "0.1", "RssSidePanel");
}

bool RssBackend::initImpl()
{
	m_rssmanager = new RssManager(this, QString("PMC Feeds"));
	connect(m_rssmanager, SIGNAL(modelPopulated(ProxyModel*)), this, SLOT(loadModel(ProxyModel*)));
	connect(m_rssmanager, SIGNAL(feedOperation(bool)), this , SLOT(feedOperation(bool)));
	return true;
}

QString RssBackend::backendCategory() const
{
	return "video";
}

bool RssBackend::expand ( int row )
{
	ProxyModel *pmodel = qobject_cast<ProxyModel*>(model());
	if(pmodel) {
		return pmodel->expand(row);
	}
	return MediaCenter::AbstractBrowsingBackend::expand(row);
}

bool RssBackend::goOneLevelUp()
{
	ProxyModel *pmodel = qobject_cast<ProxyModel*>(model());
	if(pmodel) {
		return pmodel->goOneLevelUp();
	}
	return MediaCenter::AbstractBrowsingBackend::goOneLevelUp();
}

void RssBackend::loadModel ( ProxyModel* model )
{
	setModel(model);
    emit modelRdy(true);
}

QString RssBackend::mediaBrowserSidePanel() const
{
	return m_rsspanelqml;
}

void RssBackend::setMediaBrowserSidePanel ( QString text )
{
	m_rsspanelqml = text;
	emit mediaBrowserSidePanelChanged();
}

QString RssBackend::mediaBrowserOverride() const
{
    return constructQmlSource("rsscomponents", "0.1", "MediaBrowser");
}

void RssBackend::addFeed ( const QString& feedurl )
{
	m_rssmanager->addFeed(feedurl);
}

void RssBackend::addToplist()
{
	m_rssmanager->addToplist();
}

void RssBackend::deleteFeed ( int row )
{
    m_rssmanager->deleteFeed(row);
}

void RssBackend::feedOperation ( bool result )
{
	if (result) {
		emit addFeedSuccessfull();
	} else {
		addFeedFailed();
	}
}