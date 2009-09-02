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
#ifndef MEDIABROWSERModelPackage_H
#define MEDIABROWSERModelPackage_H

#include <QObject>
#include <QVariantList>
#include "mediabrowser_export.h"

class QAbstractItemModel;

/**
 * @class MediaBrowserModelPackage @author Alessandro Diaferia
 * @brief A useful class to build custom plugins for the mediabrowser.
 *
 * This class allows you to write custom models to use with the mediabrowser applet.
 * Custom models are useful to implement custom navigations such as navigation
 * through services available through the web, see YouTube.
 *
 * This class does not direclty inherit from QAbstractItemModel due to future
 * extension purposes. It just represents an interface between the applet and the
 * provided model.
 */
class MEDIABROWSER_EXPORT MediaBrowserModelPackage : public QObject
{
    Q_OBJECT
public:
    enum BrowsingType {
        LocalBrowsing,
        RemoteBrowsing
    };

    MediaBrowserModelPackage(QObject *parent, const QVariantList &args);
    virtual ~MediaBrowserModelPackage();

    /**
     * This method must be reimplemented in order to provide the model to be
     * used by the view.
     */
    virtual QAbstractItemModel *model() = 0;

    /**
     * @return the browsing type of the model. Typically a remote browsing type
     * is specific for web services like YouTube.
     * When using a LocalBrowsing type the expected model is generally a KDirModel
     * or an inheritance of it.
     * Specifying a BrowsingType allows also using special widgets for navigations purposes
     * that allow actions like "go backward", "go forward", "go uplevel" that generally change
     * their meaning if the browsing type is local or remote.
     */
    virtual BrowsingType browsingType() = 0;
};

#endif // MEDIABROWSERModelPackage_H
