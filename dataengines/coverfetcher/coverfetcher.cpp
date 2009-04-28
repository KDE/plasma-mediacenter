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
#include "coverfetcher.h"

CoverFetcherEngine::CoverFetcherEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args), m_fetcher(new LastFMFetcher(this))
{
    connect (m_fetcher, SIGNAL(coverReady(const QString &,
                               const QString &, LastFMFetcher::CoverSize, const QPixmap &)),
             this, SLOT(getCover(const QString &,
                               const QString &, LastFMFetcher::CoverSize, const QPixmap &)));
}

CoverFetcherEngine::~CoverFetcherEngine()
{}

void CoverFetcherEngine::init()
{
}

bool CoverFetcherEngine::sourceRequestEvent(const QString &source)
{
    kDebug() << "initial checks";

    if (!source.contains("|")) {
        return false;
    }

    if (sources().contains(source)) {
        return true;
    }

    kDebug() << "fetching" << source;

    QStringList artistAlbum = source.split('|');
    if (artistAlbum.count() != 2) {
        return false;
    }

    m_fetcher->fetchCover(artistAlbum[0], artistAlbum[1], LastFMFetcher::Small);
    return true;
}

void CoverFetcherEngine::getCover(const QString &artist,
                               const QString &albumName, LastFMFetcher::CoverSize size, const QPixmap &cover)
{
    if (cover.isNull()) {
        return;
    }

    kDebug() << "setting data";
    setData(artist + "|" + albumName,
            m_fetcher->sizeToString(size), cover);
}

K_EXPORT_PLASMA_DATAENGINE(coverfetcher, CoverFetcherEngine)
