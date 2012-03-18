/*
 *   Copyright 2009 Alessandro Diaferia <alediaferia@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */
#include "coverjob.h"
#include "coverfetcher.h"

CoverJob::CoverJob(CoverFetcherEngine *engine,
             const QString &destination, const QString &operation, const QMap<QString, QVariant> &parameters,
             QObject *parent) : Plasma::ServiceJob(destination, operation, parameters, parent), m_engine(engine)
{}

void CoverJob::start()
{
    if (operationName() == "reload") {
        m_engine->reloadCover(destination());
    }
}
