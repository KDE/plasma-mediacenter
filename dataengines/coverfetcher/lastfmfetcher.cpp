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
#include "lastfmfetcher.h"

// Qt
#include <QXmlStreamReader>
#include <QImage>
#include <QPixmap>

// KDE
#include <kio/job.h>
#include <KUrl>
#include <KDebug>

LastFMFetcher::LastFMFetcher(QObject *parent) : QObject(parent)
{}

LastFMFetcher::~LastFMFetcher()
{}

void LastFMFetcher::fetchCover(const QString &artist, const QString &albumName, CoverSizes sizes)
{

    if (sizes & Small && !fetchingInProgress(albumName, Small)) {
        xmlFetch(artist, albumName, Small);
    }

    if (sizes & Medium && !fetchingInProgress(albumName, Medium)) {
       xmlFetch(artist, albumName, Medium);
    }

    if (sizes & Large && !fetchingInProgress(albumName, Large)) {
        xmlFetch(artist, albumName, Large);
    }

    if (sizes & ExtraLarge && !fetchingInProgress(albumName, ExtraLarge)) {
        xmlFetch(artist, albumName, ExtraLarge);
    }
}

bool LastFMFetcher::fetchingInProgress(const QString &album, CoverSize size)
{
    // let's check whether the requested cover is being fetched already
    foreach (const QueryAttributes value, m_queries.values()) {
        if (value.albumName == album && value.size == size) {
            return true;
        }
    }

    return false;
}

void LastFMFetcher::xmlFetch(const QString &artist, const QString &album, CoverSize size)
{
    // TODO: clean albumName removing invalid characters
    // TODO: check for errors
    const QString API_KEY("e805b9243041c25effb3fc3fe3f86983");
    const QString url = "http://ws.audioscrobbler.com/2.0/?method=album.getInfo&api_key="
                        + API_KEY
                        + "&artist="+artist
                        + "&album="+album;

    QueryAttributes attributes;
    attributes.size = size;
    attributes.albumName = album;
    attributes.artist = artist;

    KIO::TransferJob *job = KIO::get(KUrl(url), KIO::NoReload, KIO::HideProgressInfo);
    connect (job, SIGNAL(data(KIO::Job*, const QByteArray&)), this, SLOT(dataReceived(KIO::Job*, const QByteArray&)));

    m_queries[job] = attributes;
}

void LastFMFetcher::dataReceived(KIO::Job *job, const QByteArray &data)
{
    // could this ever happen?
    if (!m_queries.keys().contains(job)) {
        return;
    }

    QueryAttributes attributes = m_queries.value(job);

    QXmlStreamReader reader(data);
    while (!reader.atEnd()) {
        reader.readNext();
        if (reader.name() == "image") {
            if (reader.attributes().value("size") == sizeToString(attributes.size)) {
                QString coverUrl = reader.readElementText();

                KIO::TransferJob *coverJob = KIO::get(KUrl(coverUrl), KIO::NoReload, KIO::HideProgressInfo);
                connect (coverJob, SIGNAL(data(KIO::Job*, const QByteArray&)),
                         this, SLOT(coverReceived(KIO::Job*, const QByteArray&)));

                m_queries[coverJob] = attributes;
                break;
            }
        }
    }

    m_queries.remove(job);
}

void LastFMFetcher::coverReceived(KIO::Job *job, const QByteArray &data)
{
    // could this ever happen?
    if (!m_queries.keys().contains(job)) {
        return;
    }

    // isn't this too slow??
    QImage image = QImage::fromData(data);
    QPixmap cover = QPixmap::fromImage(image);

    kDebug() << "emitting cover ready" << cover.rect();
    emit coverReady(m_queries[job].artist, m_queries[job].albumName, m_queries[job].size, cover);

    m_queries.remove(job);
}

QString LastFMFetcher::sizeToString(CoverSize size)
{
    switch (size) {
        case LastFMFetcher::Small :
            return "small";
        case LastFMFetcher::Medium :
            return "medium";
        case LastFMFetcher::Large :
            return "large";
        case LastFMFetcher::ExtraLarge :
            return "extralarge";
        default :
            return "small";
    }
}
