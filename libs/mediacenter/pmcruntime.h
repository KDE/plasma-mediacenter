/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef PMCRUNTIME_H
#define PMCRUNTIME_H

#include <QObject>
#include <QHash>
#include <QWeakPointer>

#include "mediacenter_export.h"

class QDeclarativeEngine;
class QDeclarativeImageProvider;
class PlaylistModel;

class MEDIACENTER_EXPORT PmcRuntime : public QObject
{
    Q_OBJECT
public:
    enum RuntimeObjectType { PlaylistModel };
    explicit PmcRuntime(QHash< PmcRuntime::RuntimeObjectType, QSharedPointer< QObject > > runtimeObjects, QDeclarativeEngine* engine, QObject* parent = 0);

    /**
     * Use this method to get an instance of a particular type of PMC runtime
     * object (such as PlaylistModel).
     *
     * @param type type of the RuntimeObject you want to access
     * @return QSharedPointer to the selected runtime object
     */
    template <class T>
    QSharedPointer<T> runtimeObjectAs(PmcRuntime::RuntimeObjectType type)
    {
        return qSharedPointerObjectCast<T>(runtimeObject(type));
    }

    /**
     * Call this method from a subclass to register an image provider with QML
     *
     * @param providerId ID of the image provider. For example, if its ID is
     * "foo", images with URI "image://foo/" will use this provider
     * @param imageProvider the QDeclarativeImageProvider you want to register
     */
    void addImageProvider(const QString &providerId,
                          QDeclarativeImageProvider * provider);

private:
    class Private;
    Private * const d;

    QSharedPointer< QObject > runtimeObject(PmcRuntime::RuntimeObjectType type);
};

#endif // PMCRUNTIME_H
