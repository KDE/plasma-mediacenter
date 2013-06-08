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

#ifndef RSSBACKEND_H
#define RSSBACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>


class RssManager;
class ProxyModel;

class RssBackend : public MediaCenter::AbstractBrowsingBackend
{
	Q_OBJECT
public:
	RssBackend(QObject *parent, const QVariantList &args);
	virtual bool initImpl();
	virtual QString backendCategory() const;
	virtual bool expand(int row);
	virtual QString mediaBrowserSidePanel() const;
	virtual void setMediaBrowserSidePanel(QString text);
    virtual QString mediaBrowserOverride() const;
public:
	Q_INVOKABLE void addFeed(const QString& feedurl);
	Q_INVOKABLE void addToplist();
    Q_INVOKABLE void deleteFeed(int row);
signals:
	void addFeedSuccessfull();
	void addFeedFailed();
    void modelRdy(bool status);
public slots:
	virtual bool goOneLevelUp();
	void feedOperation( bool result );
private slots:
	void loadModel(ProxyModel* model);
private:
	RssManager* m_rssmanager;
	QString m_rsspanelqml;
};

#endif // RSSBACKEND_H
