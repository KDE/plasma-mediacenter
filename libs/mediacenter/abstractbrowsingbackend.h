/***********************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>            *
 *                                                                                 *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#ifndef ABSTRACTBROWSINGBACKEND_H
#define ABSTRACTBROWSINGBACKEND_H

#include <QObject>
#include <QVariantList>
#include <QFlags>

#include <KDE/KConfigGroup>
#include "mediacenter_export.h"
#include "mediacenter/mediacenter.h"
#include <KDE/KService>

class QDeclarativeEngine;
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
    Q_PROPERTY(QObject* backendModel READ model NOTIFY modelChanged)
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString mediaBrowserSidePanel READ mediaBrowserSidePanel WRITE setMediaBrowserSidePanel NOTIFY mediaBrowserSidePanelChanged)

public:
    /**
     * Used to load a backend instance via a plugin loader through KService.
     * The first argument of @param args must be the unique storageID of the service.
     * */
    explicit AbstractBrowsingBackend(QObject *parent, const QVariantList &args = QVariantList());
    virtual ~AbstractBrowsingBackend();

    /**
     * Sets the plugin name for this backend; used for things such as configuration group naming
     * @param name the plugin name to use
     */
    void setName(const QString &name);

    /**
     * @return the plugin name for this backend
     */
    QString name() const;

    /**
     * @returns the model of the browsing backend
     */
    QObject *model();

    /**
     * Request the backend to initialize
     */
    Q_INVOKABLE bool init();

    /**
     * Convenience function that returns the list of
     * available browsing backends found.
     */
    static KService::List availableBackends();

    /*
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

    /**
     * This slot must be called by the MediaCenter UI to set a declarive engine
     * which will then be used by the backend to set up needed values at runtime
     *
     * @param declarativeEngine pointer to the declarative engine
     */
    void setDeclarativeEngine(QDeclarativeEngine *declarativeEngine);

    /**
     * Can use this method to get access to the declarative engine
     *
     * @return pointer to declarative engine
     */
    QDeclarativeEngine *declarativeEngine() const;

    /**
     * Override this method if you want your backend to show a custom media browser
     * instead of the standard PMC MediaBrowser.
     *
     * @return Valid QML string, use constructQmlSource for convenience
     */
    Q_INVOKABLE virtual QString mediaBrowserOverride() const;

    /**
     * This method is called by PMC runtime to ask the backend to check the env and report
     * if all runtime dependencies etc are satisfied.
     * The default implementation always returns true
     *
     * @returns true if its ok to load this backend, false otherwise
     */
    virtual bool okToLoad() const;

    /**
     * This method is called by mediabrowser during runtime to ask whether backend
     * supports search or not
     * default implementaion return false
     */
    Q_INVOKABLE virtual bool supportsSearch() const;

    /**
     * This method is called by the UI when the user requests to search for a media
     *
     * @param searchTerm string entered by the user
     */
    Q_INVOKABLE virtual void search(const QString &searchTerm);

    /**
     * Override this method if you want your backend to show custom items on a
     * panel/popup. For example this can be used to set filtering options
     *
     * @return Valid QML string, use constructQmlSource for convenience
     */
     virtual QString mediaBrowserSidePanel() const;

     virtual void setMediaBrowserSidePanel(QString text);

    /**
     * Override this method and emit the busyChanged() signal to tell the mediacenter
     * that the backend is busy doing something
     *
     * @return whether the backend is busy
     */
    virtual bool busy() const;

Q_SIGNALS:
    void modelChanged();
    void busyChanged();
    void mediaBrowserSidePanelChanged();

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
     * This is a convenience function which constructs a string representing QML source for
     * loading an item of name itemName which exists in componentDirName
     *
     * @param componentDirName name of the dir (relative to org.kde.plasma.mediacenter.elements
     * @param versionString version of the item as specified in qmldir in componentDirName
     * @param itemName name of the item as specified in qmldir in componentDirName
     */
    QString constructQmlSource(const QString& componentDirName, const QString& versionString,
                               const QString& itemName) const;

    /**
     * This method is called whenever the package has been chosen by the user
     * to be the current navigation helper and just before the model is set
     * to the view.
     * Use this method for general initialization purposes.
     */
    virtual bool initImpl() = 0;


private:
    class AbstractBrowsingBackendPrivate;
    AbstractBrowsingBackendPrivate * const d;

};

} // MediaCenter namespace

#define MEDIACENTER_EXPORT_BROWSINGBACKEND( c ) \
        K_PLUGIN_FACTORY( MediaBrowserFactory, registerPlugin< c >(); ) \
        K_EXPORT_PLUGIN( MediaBrowserFactory("c") )

#endif // ABSTRACTBROWSINGBACKEND_H
