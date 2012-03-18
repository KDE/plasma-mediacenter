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

class NavigationToolbar;
class BrowserGesture;
class AbstractBrowsingBackend;

/**
 * @class Browser
 * @author Alessandro Diaferia
 * @brief The base class for the browsing applet in Plasma Media Center
 *
 * This is the base class that has to be reimplemented in order to
 * allow the user to browse through his media files both locally and remotely.
 * Different browsing backends will be used to browse through different kind of
 * media resources.
 */
class MEDIACENTER_EXPORT Browser : public Plasma::Applet
{
    Q_OBJECT
public:
    Browser(QObject *parent, const QVariantList &args);
    virtual ~Browser();

    /**
     * Enables a navigation toolbar inside the applet
     * with some common actions such as "go back", "home" .."
     * When the toolbar is enabled a vertical linear layout
     * is set as layout to the applet and the widget is inserted
     * at index 0. In order to preserve the layout it is highly recommended
     * to insert other eventual widgets from index 1 on.
     */
    void setEnableToolbar(bool);
    bool enableToolbar() const;

    /**
     * This method should only be used by the main application to
     * set the gesture type to handle.
     */
    void setGestureType(Qt::GestureType);

    /**
     * @return the toolbar widget if enableToolbar returns true
     * otherwise 0 is returned.
     */
    virtual MediaCenter::NavigationToolbar *toolbar() const;
    
    /**
     * This method must be reimplemented in order to load the browsing backend
     * and reflects the changes in the applet.
     * The most common implementation consists in applying the model supplied by
     * the backend to a view in the applet.
     */
    virtual void loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *) = 0;
    
    /**
     * This function is used to retrieve the current browsing backend in use.
     */
    MediaCenter::AbstractBrowsingBackend* currentBrowsingBackend() const;

public Q_SLOTS:
    /**
     * sets the current browsing backend and then calls loadBrowsingBackend.
     */
    void setCurrentBrowsingBackend(MediaCenter::AbstractBrowsingBackend*);

Q_SIGNALS:
    void browseUpRequest();
    void browseHistoryBackRequest();
    void browseHistoryNextRequest();
    void browsePathRequest(const QString &path);

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

protected:
    virtual bool sceneEvent(QEvent *event);
    virtual void gestureEvent(MediaCenter::BrowserGesture *);

private:
    class BrowserPrivate;
    BrowserPrivate *d;
};

} // namespace MediaCenter

#endif // BROWSER_H
