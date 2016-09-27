/*
 * Copyright 2014  Bhushan Shah <bhush94@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "qmlaccess.h"
#include "medialibrary.h"
#include "mpris2/mpris2.h"

#include <QSharedPointer>

QMLAccess::QMLAccess(QObject *parent) : QObject(parent)
{
    SingletonFactory::instanceFor<MediaLibrary>()->start();

    DataSourcesLoader datasourceLoader;
    datasourceLoader.load();

    m_playlistModel = new PlaylistModel(this);

    m_backendsModel = new BackendsModel(this);

    Mpris2 *mprisObject = new Mpris2(m_playlistModel, this);
    m_mpris2PlayerAdaptor = mprisObject->getMediaPlayer2Player();

    emit backendsModelChanged();
    emit playlistModelChanged();
    emit mpris2PlayerAdaptorChanged();
}

