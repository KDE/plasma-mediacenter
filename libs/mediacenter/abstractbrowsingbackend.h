/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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
#include <QFlags>

#include <KConfigGroup>
#include "mediacenter_export.h"
#include "mediacenter/mediacenter.h"
#include <KService>

class QAbstractItemModel;
class KConfigDialog;

namespace MediaCenter {

/**
 * @class AbstractBrowsingBackend @author Alessandro Diaferia
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
class MEDIACENTER_EXPORT AbstractBrowsingBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* backendModel READ model/* WRITE setModel*/ NOTIFY modelChanged)

public:
    enum BrowsingType {
        LocalBrowsing,
        RemoteBrowsing
    };

    enum AdditionalRoles {
        MediaRole
    };

    /**
     * Used to load a backend instance via a plugin loader through KService.
     * The first argument of @param args must be the unique storageID of the service.
     * */
    AbstractBrowsingBackend(QObject *parent, const QVariantList &args);
    AbstractBrowsingBackend(QObject *parent = 0);
    virtual ~AbstractBrowsingBackend();

    /**
     * @returns the model of the browsing backend
     */
    QObject *model();

    /**
     * @return the browsing type of the model. Typically a remote browsing type
     * is specific for web services like YouTube.
     * When using a LocalBrowsing type the expected model is generally a KDirModel
     * or an inheritance of it.
     * Specifying a BrowsingType allows the view also using special widgets for navigations purposes
     * that allow actions like "go backward", "go forward", "go uplevel" that generally change
     * their meaning if the browsing type is local or remote.
     */
    //TODO: May no longer be needed
    BrowsingType browsingType() const;

    bool hasConfigurationInterface() const;

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

    QString name() const;
    
    /**
     * Convenience function that returns the list of
     * available browsing backends found.
     */
    static KService::List availableBackends();

    /**
     * This slot must be reimplemented if browsingType returns
     * LocalBrowsing. This method will be used to list the media
     * resources in location specified by @param url
     */
    //TODO: Maybe its better to let the model handle this
    virtual void openUrl(const KUrl &url);

    /**
     * This slot must be reimplemented if browsingType returns
     * RemoteBrowsing. This method will be used to perform searches
     * for new medias to show in the browsing applet.
     */
    //TODO: Maybe its better to let the model handle this
    virtual void searchForMedia(const QString &name);

    /**
     * This function is used by Media Browser to go to one level
     * up in the browsing structure. Must be reimplemented
     *
     * @return true if operation succeeded
     * @return false if we are already at the topmost level
     */
    Q_INVOKABLE virtual bool goOneLevelUp();

    /**
     * This slot is called by the media browser when the model contains
     * directories (or equivalents of them) and the user requests to browse
     * to a particular one
     *
     * @param row the row index of the directory
     *
     * @return true if operation was successful
     * @return false if operation was unsuccessful
     */
    Q_INVOKABLE virtual bool expand(int row);

signals:
    void modelChanged(QAbstractItemModel * model);

protected:
    /**
     * This method must be set in order to provide the model to be
     * used by the view.
     * @note When LocalBrowsing is set it is highly recommended to use a KDirModel.     ////TODO: May no longer be needed
     * If the package is set to RemoteBrowsing the model
     * must provide an url that points to the media content for each
     * QModelIndex. It must make use of MediaRole role to accomplish this.
     * @see MediaRole
     */
    void setModel(QAbstractItemModel * model);
    
    /**
     * As from Plasma::Applet this method should be used in order
     * to inform about the availability of a configuration interface for
     * the model.
     */
    void setHasConfigurationInterface(bool hasInterface);

    void setBrowsingType(BrowsingType);

private:
    class AbstractBrowsingBackendPrivate;
    AbstractBrowsingBackendPrivate *d;

};

} // MediaCenter namespace

#define MEDIACENTER_EXPORT_BROWSINGBACKEND( c ) \
        K_PLUGIN_FACTORY( MediaBrowserFactory, registerPlugin< c >(); ) \
        K_EXPORT_PLUGIN( MediaBrowserFactory("c") )

#endif // MODELPACKAGE_H
