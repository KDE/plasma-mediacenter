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
#ifndef LASTFMFETCHER_H
#define LASTFMFETCHER_H

#include <QObject>
#include <QHash>

namespace KIO {
    class Job;
}
class QByteArray;
class QPixmap;

class LastFMFetcher : public QObject
{
    Q_OBJECT
public:
    enum CoverSize {
        Invalid = 0x0,
        Small = 0x1,
        Medium = 0x2,
        Large = 0x4,
        ExtraLarge = 0x8,
        AllSizes = Small | Medium | Large | ExtraLarge
    };
    Q_DECLARE_FLAGS(CoverSizes, CoverSize)

    LastFMFetcher(QObject *parent = 0);
    ~LastFMFetcher();

    void fetchCover(const QString &artist, const QString &albumName, CoverSize size);
    static QString sizeToString(CoverSize size);

protected slots:
    void dataReceived(KIO::Job*, const QByteArray &data);
    void coverReceived(KIO::Job*, const QByteArray &data);

signals:
    void coverReady(const QString &artist, const QString &albumName, LastFMFetcher::CoverSize size, const QPixmap &cover);

private:
    typedef struct QueryAttributes {
        CoverSize size;
        QString albumName;
        QString artist;
    } QueryAttributes;
    QHash<KIO::Job*, QueryAttributes> m_queries;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(LastFMFetcher::CoverSizes)

#endif
