#ifndef ITEMCACHE_H
#define ITEMCACHE_H

#include <QSharedPointer>
#include <QHash>

class Album;
class Artist;

class ItemCache
{
public:
    QSharedPointer<Artist> getArtistByName(const QString &name, bool create = true);
    QSharedPointer<Album> getAlbumByName(const QString &albumName,
                                         const QString &artistName,
                                         bool create = true);
private:
    QHash<QString, QSharedPointer<Artist>> m_artists;
    QHash<QPair<QString,QString>, QSharedPointer<Album>> m_albums;
};

#endif // ITEMCACHE_H
