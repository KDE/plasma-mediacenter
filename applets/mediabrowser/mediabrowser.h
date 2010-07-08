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
#ifndef MEDIABROWSER_H
#define MEDIABROWSER_H

#include <mediacenter/browser.h>

#include "ui_general.h"
#include "abstractmediaitemview.h"

#include <QGraphicsLinearLayout>

class BrowsingWidget;
class AbstractBrowsingBackend;

class MediaBrowser : public MediaCenter::Browser
{
    Q_OBJECT
public:
    enum BrowsingMode { LocalFiles, RemoteSearches };
     MediaBrowser(QObject *parent, const QVariantList &args);
    ~MediaBrowser();

    void init();
    void createConfigurationInterface(KConfigDialog *parent);

    void setShowingBrowsingWidgets(bool);
    bool isShowingBrowsingWidgets() const;

    void showStartupState();

    void addViewMode(const QString &title);
    QStringList viewModes() const;

    QList<MediaCenter::Media> selectedMedias() const;

    KUrl directory() const;

public slots:
    void openDirectory(const KUrl &url);
    void listMediaInDirectory();
    void selectedMediasAdd(const MediaCenter::Media &media);
    void selectedMediasRemove(const MediaCenter::Media &media);
    void clearSelectedMedias();

    void clearViewModes();

protected:
    void gestureEvent(MediaCenter::BrowserGesture *);

signals:
    void mediasListChanged(const QList<MediaCenter::Media> &list);

    void musicDataEngine();
    void videoDataEngine();
    void pictureDataEngine();

private:
    AbstractMediaItemView *m_view;
    Ui::General uiGeneral;

    QAbstractItemModel *m_model;

    bool m_blurred;
    QString m_viewType;
    BrowsingWidget *m_browsingWidget;
    bool m_browsingWidgets;
    AbstractBrowsingBackend *m_backend;
    QList<MediaCenter::Media> m_selectedMedias;

    QGraphicsLinearLayout *m_layout;

private:
    void createView();
    void showInstalledBackends();

private slots:
    void loadConfiguration();
    void configAccepted();
    void slotIndexActivated(const QModelIndex &);

};

#endif
