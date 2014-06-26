#include "itemcache.h"
#include "artist.h"
#include "album.h"

QSharedPointer< Album > ItemCache::getAlbumByName(const QString& albumName,
                                                  const QString& artistName,
                                                  bool create)
{
    auto albumArtistPair = QPair<QString,QString>(albumName, artistName);

    if (!m_albums.contains(albumArtistPair) && create) {
        m_albums.insert(QPair<QString,QString>(albumName, artistName),
                        QSharedPointer<Album>(new Album(albumName, getArtistByName(artistName))));
    }

    return m_albums.value(albumArtistPair);
}

QSharedPointer< Artist > ItemCache::getArtistByName(const QString& name, bool create)
{
    if (!m_artists.contains(name) && create) {
        m_artists.insert(name, QSharedPointer<Artist>(new Artist(name)));
    }

    return m_artists.value(name);
}
