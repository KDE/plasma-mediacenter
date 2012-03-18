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
#include "coverservice.h"

CoverFetcherEngine::CoverFetcherEngine(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args), m_fetcher(new LastFMFetcher(this)),
    m_cache(KPixmapCache("mccovers"))
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
    if (!source.contains("|")) {
        return false;
    }

    if (sources().contains(source)) {
        return true;
    }

    QStringList artistAlbum = source.split('|');
    if (artistAlbum.count() != 2) {
        return false;
    }

    LastFMFetcher::CoverSizes readySizes = 0x0;
    QPixmap pixmap;
    // we check whether we have a cache copy of the cover for each
    // size type we support
    if (m_cache.find(source + "|small", pixmap)) {
        kDebug() << "small in cache";
        setData(source, "small", pixmap);
        readySizes |= LastFMFetcher::Small;
    }
    if (m_cache.find(source + "|medium", pixmap)) {
        kDebug() << "medium in cache";
        setData(source, "medium", pixmap);
        readySizes |= LastFMFetcher::Medium;
    }
    if (m_cache.find(source + "|large", pixmap)) {
        kDebug() << "large in cache";
        setData(source, "large", pixmap);
        readySizes |= LastFMFetcher::Large;
    }
    if (m_cache.find(source +"|extralarge", pixmap)) {
        kDebug() << "extralarge in cache";
        setData(source, "extralarge", pixmap);
        readySizes |= LastFMFetcher::ExtraLarge;
    }
    // all keys loaded
    if (readySizes & LastFMFetcher::AllSizes) {
        return true;
    }

    if (!(readySizes & LastFMFetcher::Small)) {
        kDebug() << "small not present, fetching";
        m_fetcher->fetchCover(artistAlbum[0], artistAlbum[1], LastFMFetcher::Small);
    }
    if (!(readySizes & LastFMFetcher::Medium)) {
        kDebug() << "medium not present, fetching";
        m_fetcher->fetchCover(artistAlbum[0], artistAlbum[1], LastFMFetcher::Medium);
    }
    if (!(readySizes & LastFMFetcher::Large)) {
        kDebug() << "large not present, fetching";
        m_fetcher->fetchCover(artistAlbum[0], artistAlbum[1], LastFMFetcher::Large);
    }
    if (!(readySizes & LastFMFetcher::ExtraLarge)) {
        kDebug() << "extralarge not present, fetching";
        m_fetcher->fetchCover(artistAlbum[0], artistAlbum[1], LastFMFetcher::ExtraLarge);
    }

    setData(source, Plasma::DataEngine::Data());
    return true;
}

void CoverFetcherEngine::getCover(const QString &artist,
                               const QString &albumName, LastFMFetcher::CoverSize size, const QPixmap &cover)
{
    if (cover.isNull()) {
        return;
    }

    kDebug() << "setting cover for" << artist << albumName;

    m_cache.insert(artist + "|" + albumName + "|" + LastFMFetcher::sizeToString(size), cover);

    setData(artist + "|" + albumName,
            LastFMFetcher::sizeToString(size), cover);
}

void CoverFetcherEngine::reloadCover(const QString &source)
{
    kDebug() << "";
    if (!sources().contains(source)) {
        return;
    }
    kDebug() << "reloading";

    const QString artist = source.split("|")[0];
    const QString album = source.split("|")[1];

    m_fetcher->fetchCover(artist, album, LastFMFetcher::AllSizes);

}

Plasma::Service* CoverFetcherEngine::serviceForSource(const QString &source)
{
    if (!sources().contains(source)) {
        return 0;
    }

    Plasma::Service *service = new CoverService(this, this);
    service->setDestination(source);

    return service;
}

K_EXPORT_PLASMA_DATAENGINE(coverfetcher, CoverFetcherEngine)
