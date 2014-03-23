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
#include <KDE/KService>

#include "mediacenter_export.h"
#include "mediacenter/mediacenter.h"

class QDeclarativeEngine;
class QAbstractItemModel;
class KConfigDialog;
class ModelMetadata;

namespace MediaCenter {

/**
 * @class AbstractBrowsingBackend @author Alessandro Diaferia
 * @brief A useful class to build custom plugins for the mediabrowser.
 *
 * This class allows you to write custom models to use with the mediabrowser applet.
 * Custom models are useful to implement custom navigations such as navigation
 * through services available through the web, see YouTube.
 *
 * This class does not directly inherit from QAbstractItemModel due to future
 * extension purposes. It just represents an interface between the applet and the
 * provided model.
 */
class MEDIACENTER_EXPORT AbstractBrowsingBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(QString mediaBrowserSidePanel READ mediaBrowserSidePanel NOTIFY mediaBrowserSidePanelChanged)
    Q_PROPERTY(QVariantList models READ models NOTIFY modelsChanged)
    Q_PROPERTY(QVariantList buttons READ buttons NOTIFY buttonsChanged)
    Q_PROPERTY(QObject* pmcRuntime READ pmcRuntime WRITE setPmcRuntime NOTIFY pmcRuntimeChanged)

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
     * The default implementation returns the first (or the only) model.
     * @returns the model of the browsing backend.
     */
    QAbstractItemModel *model();

    /**
     * @returns the models available in the browsing backend
     */
    QVariantList models();

    /**
     * When writing a backend, override this method to return a list of strings
     * that will be used to create buttons when this backend is chosen.
     *
     * When a button is pressed, the handleButtonClick method will be called.
     *
     * @see handleButtonClick()
     * @returns a list of buttons the model wants to show on the UI
     */
    virtual QVariantList buttons();

    /**
     * This method will be called when the media center wants to load this backend.
     * Please use initImpl method to do initialization instead of the constructor.
     * @see initImpl
     */
    Q_INVOKABLE bool init();

    /**
     * Convenience function that returns the list of
     * available browsing backends found.
     */
    static KService::List availableBackends();

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
     * @param model the model that the backend should operate on
     *
     * @return true if operation was successful
     * @return false if operation was unsuccessful
     */
    Q_INVOKABLE virtual bool expand(int row, QAbstractItemModel *model);

    /**
     * This is a convenience method for when the media browser thinks the model
     * doesn't matter. In this case, the default model is assumed by the backend.
     *
     * @param row the row index of the directory
     *
     * @see model()
     * @return true if operation was successful
     * @return false if operation was unsuccessful
     */
    Q_INVOKABLE virtual bool expand(int row);

    /**
     * This slot must be called by the MediaCenter UI to set a declarive engine
     * which can then be used by the backend to set up needed values at runtime
     *
     * @param declarativeEngine pointer to the declarative engine
     */
    void setDeclarativeEngine(QDeclarativeEngine *declarativeEngine);

    /**
     * Use this method to get access to the declarative engine.
     * This is useful for backends to provide their own image providers and so on
     *
     * @return the declarative engine
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
     * Override this method if you want your backend to show custom items on a
     * panel/popup. For example this can be used to set filtering options
     *
     * @return Valid QML string, use constructQmlSource for convenience
     */
     virtual QString mediaBrowserSidePanel() const;

    /**
     * Override this method and emit the busyChanged() signal to tell the mediacenter
     * that the backend is busy doing something
     *
     * @return whether the backend is busy
     */
    virtual bool busy() const;

    /**
     * This method will be called by mediacenter when one of the buttons is clicked.
     * The default implementation does nothing
     *
     * @param buttonName the name of the button that was clicked
     */
    Q_INVOKABLE virtual void handleButtonClick(const QString&);

    //FIXME: Bad, bad, bad.
    QObject* pmcRuntime();
    void setPmcRuntime(QObject* pmcRuntime);

Q_SIGNALS:
    void busyChanged();
    void mediaBrowserSidePanelChanged();
    void modelsChanged();
    void buttonsChanged();
    void searchTermChanged();

    void error(const QString &message);
    void modelNeedsAttention(QObject* model);
    void pmcRuntimeChanged();

protected:
    /**
     * Set the only model (and its metadata) that this backend contains.
     * This used by the media browser to show media.
     */
    void setModel(ModelMetadata* model);

    /**
     * Set the only model that this backend contains. Default metadata will be
     * defined every time.
     * This used by the media browser to show media.
     */
    void setModel(QAbstractItemModel* model);

    /**
     * This method is used to add more models to this backend
     */
    void addModel(ModelMetadata * model);

    /**
     * This method adds multiple models as a single model collection.
     * This indicates the UI to show views for these models simultaneously
     */
    void addModelPair(const QString& pairLabel, QObject* firstModel, QObject* secondModel);

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
     * This method is called whenever the backend has been chosen by the user
     * to be the current navigation helper and just before the model is set
     * to the view.
     * Use this method for general initialization purposes.
     */
    virtual bool initImpl() = 0;

    /**
     * This method is called by the UI when the user requests to search for a media.
     * You should override this method if your backend support search and has
     * only one model
     *
     * @param searchTerm string entered by the user
     */
    Q_INVOKABLE virtual void search(const QString &searchTerm);

    /**
     * This method is called by the UI when the user requests to search for a media
     * for a particular model.
     * You should override this method if your backend support search and has
     * multiple models
     *
     * @param searchTerm string entered by the user
     * @param model the model that this search should affect
     */
    Q_INVOKABLE virtual void searchModel(const QString &searchTerm, QAbstractItemModel *model);

private:
    class AbstractBrowsingBackendPrivate;
    AbstractBrowsingBackendPrivate * const d;

};

} // MediaCenter namespace

Q_DECLARE_METATYPE(QAbstractItemModel*)

#define MEDIACENTER_EXPORT_BROWSINGBACKEND( c ) \
        K_PLUGIN_FACTORY( MediaBrowserFactory, registerPlugin< c >(); ) \
        K_EXPORT_PLUGIN( MediaBrowserFactory("c") )

#endif // ABSTRACTBROWSINGBACKEND_H
