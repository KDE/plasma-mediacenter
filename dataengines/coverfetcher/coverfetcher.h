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
#ifndef COVERFETCHER_H
#define COVERFETCHER_H

#include <Plasma/DataEngine>

#include "lastfmfetcher.h"

class CoverFetcherEngine : public Plasma::DataEngine
{
    Q_OBJECT
public:
    CoverFetcherEngine(QObject *parent, const QVariantList &args);
    ~CoverFetcherEngine();

    void init();
    Plasma::Service* serviceForSource(const QString &source);

protected:
    bool sourceRequestEvent(const QString &source);

protected slots:
    void getCover(const QString &artist, const QString &albumName, LastFMFetcher::CoverSize size, const QPixmap &cover);

public slots:
    void reloadCover(const QString &source);

private:
    LastFMFetcher *m_fetcher;
};

#endif
