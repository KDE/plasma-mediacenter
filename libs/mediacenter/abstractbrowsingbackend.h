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
#include <QAbstractItemModel>

#include <KConfigGroup>
#include <KPluginInfo>

#include "mediacenter_export.h"
#include "mediacenter.h"

class PmcModel;

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
    Q_PROPERTY(QObject* models READ models NOTIFY modelsChanged)
    Q_PROPERTY(QStringList buttons READ buttons NOTIFY buttonsChanged)

public:
    /**
     * Used to load a backend instance via a plugin loader through KService.
     */
    explicit AbstractBrowsingBackend(QObject *parent, const QVariantList &args = QVariantList());
    virtual ~AbstractBrowsingBackend();

    /**
     * @return the plugin name for this backend
     */
    QString name() const;

    /**
     * @return the icon for this backend, as set in desktop file
     */
    QString icon() const;

    /**
     * @return the comment for this backend, as set in desktop file
     */
    QString comment() const;

    /**
     * @return the category of this backend
     */
    QString category() const;

    /**
     * The default implementation returns the first (or the only) model.
     * @returns the model of the browsing backend
     */
    QAbstractItemModel *model();

    /**
     * @returns the models available in the browsing backend
     */
    QObject* models();

    /**
     * Call this method to get a list of strings that should be used to create
     * buttons when this backend is chosen.
     *
     * When a button is pressed, the handleButtonClick method will be called.
     *
     * @see handleButtonClick()
     * @returns a list of buttons the backend wants to show on the UI
     */
    QStringList buttons() const;

    /**
     * This method will be called when the media center wants to load this backend.
     * Please use initImpl method to do initialization instead of the constructor.
     * @see initImpl
     */
    Q_INVOKABLE bool init();

    /**
     * Sets pluginInfo on abstract browsing backend
     */
     void setPluginInfo(const KPluginInfo& info);

    /**
     * This function is used by Media Browser to go to one level
     * up in the browsing structure. Must be reimplemented
     *
     * @deprecated
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

    /**
     * Override this method to provide all media that should be added to playlist when playAll
     * button is clicked
     *
     * @return list of media urls
     */
    Q_INVOKABLE virtual QStringList allMedia();

Q_SIGNALS:
    void busyChanged();
    void modelsChanged();
    void buttonsChanged();
    void searchTermChanged();

    void error(const QString &message);
    void pmcRuntimeChanged();
    void showCustomUi(const QString &customUiQml);

public Q_SLOTS:
    bool back(QObject* model);

protected:
    /**
     * Set the only model (and its metadata) that this backend contains.
     * This used by the media browser to show media.
     */
    void setModel(PmcModel* model);

    /**
     * Set the only model that this backend contains. Default metadata will be
     * defined every time.
     * This used by the media browser to show media.
     */
    void setModel(QAbstractItemModel* model);

    /**
     * This method is used to add more models to this backend
     */
    void addModel(PmcModel * model);

    /**
     * Use this method to replace a model that has already been added
     *
     * @param original the model that has been added already
     * @param replacement the model to replace the original with
     *
     * @return true if successful, false otherwise
     */
    bool replaceModel(PmcModel *original, PmcModel *replacement);

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
     * Subclasses can call this method to set the button strings that are used
     * for the buttons property
     *
     * @param buttons list of strings for the buton labels
     */
    void setButtons(const QStringList &buttons);

private:
    class AbstractBrowsingBackendPrivate;
    AbstractBrowsingBackendPrivate * const d;
};

} // MediaCenter namespace

Q_DECLARE_METATYPE(QAbstractItemModel*)

#define MEDIACENTER_EXPORT_BROWSINGBACKEND(classname, jsonfile) \
        K_PLUGIN_FACTORY_WITH_JSON(MediaBrowserFactory, jsonfile, registerPlugin<classname>();)

#endif // ABSTRACTBROWSINGBACKEND_H
