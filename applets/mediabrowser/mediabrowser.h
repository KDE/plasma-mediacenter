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

#include <Plasma/Applet>
#include "ui_localconfig.h"
#include "abstractmediaitemview.h"

class KUrl;
class KDirLister;

class MediaBrowser : public Plasma::Applet
{
    Q_OBJECT
public:
    enum BrowsingMode { LocalFiles, RemoteSearches };
     MediaBrowser(QObject *parent, const QVariantList &args);
    ~MediaBrowser();

    void init();
    void createConfigurationInterface(KConfigDialog *parent);

    void switchToFileModel();

private:
    AbstractMediaItemView *m_listView;
    Ui::LocalConfig uiLocal;

    KUrl m_localUrl;
    bool m_fromPlaces;
    BrowsingMode m_mode;
    KDirLister *m_lister;

private slots:
    void loadConfiguration();
    void configAccepted();

};

#endif
