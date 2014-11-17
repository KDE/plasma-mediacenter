/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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

#ifndef METADATAMUSICBACKEND_H
#define METADATAMUSICBACKEND_H

#include "../abstractmetadatabackend.h"

class FilterMediaModel;
class PmcMetadataModel;
class AlwaysExpandedMetadataModel;
class PmcMetadataModel;
class ModelMetadata;

class MetadataMusicBackend : public AbstractMetadataBackend
{
    Q_OBJECT
    Q_PROPERTY(QString artistFilter READ artistFilter WRITE setArtistFilter NOTIFY artistFilterChanged)
    Q_PROPERTY(QString albumFilter READ albumFilter WRITE setAlbumFilter NOTIFY albumFilterChanged)

public:
    MetadataMusicBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataMusicBackend();

    QString artistFilter() const;
    QString albumFilter() const;

    void setArtistFilter(const QString &filter);
    void setAlbumFilter(const QString &filter);

    void addAllSongsToPlaylist( const QSharedPointer< PlaylistModel >& playlistModel );

    virtual bool expand(int row, QAbstractItemModel* model);

    virtual void handleButtonClick(const QString& buttonName);

    virtual void searchModel(const QString& searchTerm, QAbstractItemModel* model);

    virtual QStringList allMedia();

Q_SIGNALS:
    void artistFilterChanged();
    void albumFilterChanged();

protected:
    void updateModelAccordingToFilters();
    virtual bool initImpl();

private slots:

    void initializeModels();

private:
    AlwaysExpandedMetadataModel* m_artistsModel;
    AlwaysExpandedMetadataModel* m_albumsModel;
    PmcMetadataModel* m_musicModel;
    PlaylistModel* m_playlistModel;

    QString m_artistFilter;
    QString m_albumFilter;
    ModelMetadata* m_musicModelMetadata;
    ModelMetadata* m_albumsModelMetadata;
    ModelMetadata* m_artistsModelMetadata;
    FilterMediaModel* m_musicFilteredModel;
    FilterMediaModel* m_albumFilteredModel;
    FilterMediaModel* m_artistFilteredModel;
};

#endif // METADATAMUSICBACKEND_H
