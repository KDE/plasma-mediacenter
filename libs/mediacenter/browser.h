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
#ifndef BROWSER_H
#define BROWSER_H

#include "mediacenter_export.h"

#include <Plasma/Applet>
#include <mediacenter/mediacenter.h>


namespace MediaCenter {

class MEDIACENTER_EXPORT Browser : public Plasma::Applet
{
    Q_OBJECT
public:
    Browser(QObject *parent, const QVariantList &args);
    virtual ~Browser();

    virtual void setShowingBrowsingWidgets(bool) = 0;
    virtual bool isShowingBrowsingWidgets() const = 0;

    virtual void addViewMode(const QString &) = 0;
    virtual QStringList viewModes() const = 0;

    virtual void showStartupState() = 0;
    virtual QList<MediaCenter::Media> selectedMedias() const = 0;

    virtual KUrl directory() const = 0;

public Q_SLOTS:
    virtual void openDirectory(const KUrl &url) = 0;
    virtual void listMediaInDirectory() = 0;
    virtual void selectedMediasAdd(const MediaCenter::Media &) = 0;
    virtual void selectedMediasRemove(const MediaCenter::Media &) = 0;
    virtual void clearSelectedMedias() = 0;

    virtual void clearViewModes() = 0;

Q_SIGNALS:
    void mediasActivated(const QList<MediaCenter::Media> &);

    /**
     * This signal should be emitted whenever the current list
     * of shown medias in the browser changes
     */
    void mediasListChanged(const QList<MediaCenter::Media> &);
    void mediaActivated(const MediaCenter::Media &);

    /**
     * This signal should be emitted as soon as the selection of items changes
     */
    void selectedMediasChanged(const QList<MediaCenter::Media> &);

   /**
    * The browser should emit these signals when a certain dataengine type is selected
    * to inform the state machine to switch to the corresponding state.
    */
    void musicDataEngine();
    void videoDataEngine();
    void pictureDataEngine();
    //TODO: Replace these with one signal that contains a pointer to the engine
    //This pointer we can then use in the states (useful for example when we enter a state via the background buttons
    //to load the last engine). But we need to have the modelpackage library in the general libraries.
};

} // namespace MediaCenter

#endif // BROWSER_H
