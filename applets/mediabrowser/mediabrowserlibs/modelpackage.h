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
#ifndef MODELPACKAGE_H
#define MODELPACKAGE_H

#include <QObject>
#include <QVariantList>
#include <KConfigGroup>
#include "mediabrowser_export.h"
#include "mediacenter/mediacenter.h"

class QAbstractItemModel;
class KConfigDialog;

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
class MEDIABROWSER_EXPORT ModelPackage : public QObject
{
    Q_OBJECT
public:
    enum BrowsingType {
        LocalBrowsing,
        RemoteBrowsing
    };

    enum AdditionalRoles {
    	MediaRole
    };

    ModelPackage(QObject *parent, const QVariantList &args);
    virtual ~ModelPackage();

    /**
     * This method must be reimplemented in order to provide the model to be
     * used by the view.
     * If the package is set to RemoteBrowsing the model
     * must provide an url that points to the media content for each
     * QModelIndex. It must make use of MediaRole role to accomplish this.
     * @see MediaRole
     */
    virtual QAbstractItemModel *model() = 0;

    /**
     * @return the browsing type of the model. Typically a remote browsing type
     * is specific for web services like YouTube.
     * When using a LocalBrowsing type the expected model is generally a KDirModel
     * or an inheritance of it.
     * Specifying a BrowsingType allows the view also using special widgets for navigations purposes
     * that allow actions like "go backward", "go forward", "go uplevel" that generally change
     * their meaning if the browsing type is local or remote.
     */
    virtual BrowsingType browsingType() const = 0;

    bool hasConfigurationInterface();

    /**
     * As from Plasma::Applet this method should be reimplemented
     * in order to provide a useful configuration interface for
     * the model if needed.
     * @note: the method setHasConfigurationInterface() has to be
     * called in order for this function to be called by the mediabrowser.
     * @see setHasConfigurationInterface
     */
    virtual void createConfigurationInterface(KConfigDialog *parent);

    /**
     * @return the appropriate KConfigGroup for this plugin.
     */
    KConfigGroup config();

    /**
     * This method is called whenever the package has been chosen by the user
     * to be the current navigation helper and just before the model is set
     * to the view.
     * Use this method for general initialization purposes.
     */
    virtual void init();

    MediaCenter::MediaTypes allowedMediaTypes();

protected:
    /**
     * As from Plasma::Applet this method should be used in order
     * to inform about the availability of a configuration interface for
     * the model.
     */
    void setHasConfigurationInterface(bool hasInterface);
    void setAllowedMediaTypes(const MediaCenter::MediaTypes &type);

private:
    class ModelPackagePrivate;
    ModelPackagePrivate *d;

    MediaCenter::MediaTypes m_allowedMediaTypes;
};

#endif // MODELPACKAGE_H
