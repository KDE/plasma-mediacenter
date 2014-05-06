/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>
    Copyright (C) 2011  Shantanu Tushar <shantanu@kde.org>
    Copyright (C) 2013  Akshay Ratan  <akshayratan@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef LOCALFILESABSTRACTBACKEND_H
#define LOCALFILESABSTRACTBACKEND_H

#include <mediacenter/abstractbrowsingbackend.h>

#include <KDE/KFilePlacesModel>
#include <Solid/Device>
#include <Solid/StorageAccess>

/**
 *Forward declarations to tell the compiler that an
 *instance will be declared of LocalPlacesModel and LocalFilesAbstractModel
 */

class PlaylistModel;
class LocalPlacesModel;
class LocalFilesAbstractModel;

/**
 * @class LocalFilesAbstractBackend @author Alessandro Diaferia @author Shantanu Tushar @author Akshay Ratan
 * @brief A useful backend class for the mediabrowser.
 *
 * @description This class supports and handles LocalPlacesModel and LocalFilesModel and is useful for the
 * purpose of making changes to the mediabrowser functioning.
 *
 * This class direclty inherit from AbstractBrowsingBackend which is an interface between an applet
 * and a custom provided model.
 */

class LocalFilesAbstractBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT

public:

    /**
     * Constructor
     * Used to load backend instances
     * @param parent is a QObject pointer
     * The first argument of @param args must be the unique storageID of the KService
     */

    LocalFilesAbstractBackend (QObject* parent, const QVariantList& args);

    /**
     * Used to go one level back/up the current mediabrowser page
     */
    virtual bool goOneLevelUp();

    /**
     * This function expands the folder and show its contents by calling browseTo function accessed by LocalFilesAbstractModel
     * Sets up the needed values at the runtime
     * @param row is the number used to pass in the model as a row number
     */
    virtual bool expand (int row);

    /** @return the pointer to the LocalPlacesModel
     * Initializes the LocalPlacesModel
     */
    Q_INVOKABLE QObject *placesModel();

    /**
     * @return boolean value depending whether the disk is mounted or not
     * Used for browsing disk which is not yet mounted
     */
    bool browseToPlace(int row);

    /**
     * Calls the goOneLevelUp() function
     */

    Q_INVOKABLE void browseOneLevelUp();

    /**
     * Uses the KConfigService
     * @returns true if its ok to load this backend, false otherwise
     */
    virtual bool okToLoad() const;

    virtual QVariantList buttons();
    virtual void handleButtonClick(const QString& buttonName);
    void addAllSongsToPlaylist( const QSharedPointer< PlaylistModel >& playlistModel);

protected:

  /**
     * Used for general initialization purposes
     * @returns true
     */
    virtual bool initImpl();

    /**
     * Used to initialize local models
     */
    virtual void initModel() = 0;
    LocalPlacesModel *m_placesModel;

private slots:

    /**
     * Calls expand(int row) function after being connected by a signal in browseToPlace function
     */
    void slotStorageSetupDone(Solid::ErrorType error, const QVariant& errordata, const QString& udi);

private:
    int m_placesRow;
    bool m_isShowingPlacesModel;
    void setShowingPlacesModel(bool showing);
};

#endif // LOCALFILESABSTRACTBACKEND_H
