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
#include <Plasma/DeclarativeWidget>

class QGraphicsLinearLayout;
class AbstractMediaItemView;

namespace MediaCenter {
    class AbstractBrowsingBackend;
}

class MediaBrowser : public MediaCenter::Browser
{
    Q_OBJECT
public:
    enum BrowsingMode { LocalFiles, RemoteSearches };
     MediaBrowser(QObject *parent, const QVariantList &args);
    ~MediaBrowser();

    void init();
    void createConfigurationInterface(KConfigDialog *parent);

    QList<MediaCenter::Media> selectedMedias() const;

    KUrl currentUrl() const;

    void loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend* backend);

public slots:
    void openUrl(const KUrl& url);
    void listMediaInDirectory();
    void selectedMediasAdd(const MediaCenter::Media &media);
    void selectedMediasRemove(const MediaCenter::Media &media);
    void clearSelectedMedias();

signals:
    void mediasListChanged(const QList<MediaCenter::Media> &list);

    void musicDataEngine();
    void videoDataEngine();
    void pictureDataEngine();

private:
    Plasma::DeclarativeWidget * m_view;

    MediaCenter::AbstractBrowsingBackend *m_backend;

    bool m_blurred;
    QString m_viewType;
    QList<MediaCenter::Media> m_selectedMedias;
    QList<QObject*> m_backends;

    QGraphicsLinearLayout *m_layout;

private:
    void showInstalledBackends();

};

#endif
