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
#ifndef PLAYLISTJOB_H
#define PLAYLISTJOB_H

#include <Plasma/ServiceJob>
#include <QMap>

class PlaylistEngine;

class PlaylistJob : public Plasma::ServiceJob
{
    Q_OBJECT
public:
    PlaylistJob(PlaylistEngine *engine,
        const QString &destination, const QString &operation, const QMap<QString, QVariant> &params, QObject *parent = 0);

    void start();

private:
    PlaylistEngine *m_engine;
};

#endif
