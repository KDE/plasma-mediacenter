/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
 *   Copyright 2007 Aaron Seigo <aseigo@kde.org>
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


#include "metadatamusicbackend.h"
#include "alwaysexpandedmetadatamodel.h"
#include "../metadatabackendcommonmodel.h"

#include <libs/mediacenter/pmcmetadatamodel.h>
#include <libs/mediacenter/playlistmodel.h>

#include <Nepomuk2/Vocabulary/NMM>
#include <Nepomuk2/Vocabulary/NFO>
#include <Nepomuk2/Query/AndTerm>
#include <Nepomuk2/Query/ComparisonTerm>
#include <Nepomuk2/Query/ResourceTypeTerm>
#include <Nepomuk2/Query/ResourceTerm>

#include <KDebug>

#include <QDeclarativeEngine>
#include <QDeclarativeContext>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend)

namespace {
    static const char *s_showAllButton = "Show All";
    static const char *s_playAllButton = "Play All";
}

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
    , m_artistsModel(0)
    , m_albumsModel(0)
    , m_musicModel(0)
    , m_playlistModel(0)
    , m_shallAddMediaToPlaylist(false)
{
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

bool MetadataMusicBackend::initImpl()
{
    AbstractMetadataBackend::initImpl();
    m_albumsModel = new AlwaysExpandedMetadataModel(this);
    m_artistsModel = new AlwaysExpandedMetadataModel(this);
    m_musicModel = new MetadataBackendCommonModel(this);
    m_artistFilteredMusicModel = new MetadataBackendCommonModel(this);
    m_artistsModel->setDefaultDecoration("user-identity");
    m_albumsModel->setDefaultDecoration("media-optical-audio");
    m_albumsModel->showMediaForProperty(Nepomuk2::Vocabulary::NMM::musicAlbum());
    m_artistsModel->showMediaForProperty(Nepomuk2::Vocabulary::NMM::performer());
    m_musicModel->showMediaType(MediaCenter::Music);
    m_artistFilteredMusicModel->showMediaType(MediaCenter::Music);
    connect(m_musicModel, SIGNAL(modelReset()), SLOT(musicModelReset()));

    m_albumsModel->metadata()->setName("Albums");
    m_artistsModel->metadata()->setName("Artists#list");
    m_musicModel->metadata()->setName("Songs#list");
    m_artistFilteredMusicModel->metadata()->setName("Artist's Songs#list");

    m_musicModel->metadata()->setSupportsSearch(true);
    m_artistFilteredMusicModel->metadata()->setSupportsSearch(true);
    m_artistsModel->metadata()->setSupportsSearch(true);
    m_albumsModel->metadata()->setSupportsSearch(true);

    addModel(m_musicModel);
    addModel(m_albumsModel);
    addModelPair("Artists", m_artistsModel, m_artistFilteredMusicModel);

    updateModelAccordingToFilters();
    return true;
}

bool MetadataMusicBackend::supportsSearch() const
{
    return false;
}

QString MetadataMusicBackend::albumFilter() const
{
    return m_albumFilter;
}

QString MetadataMusicBackend::artistFilter() const
{
    return m_artistFilter;
}

void MetadataMusicBackend::setAlbumFilter(const QString& filter)
{
    m_albumFilter = filter;
    emit albumFilterChanged();
    m_artistFilter.clear();
    emit artistFilterChanged();
    updateModelAccordingToFilters();
}

void MetadataMusicBackend::setArtistFilter(const QString& filter)
{
    m_artistFilter = filter;
    emit artistFilterChanged();
    m_albumFilter.clear();
    emit albumFilterChanged();
    updateModelAccordingToFilters();
}

void MetadataMusicBackend::updateModelAccordingToFilters()
{
    m_musicModel->clearAllFilters();
    m_artistFilteredMusicModel->clearAllFilters();

    if (!m_albumFilter.isEmpty()) {
        m_musicModel->addFilter(Nepomuk2::Vocabulary::NMM::musicAlbum(), Nepomuk2::Query::ResourceTerm(m_albumFilter));
    }
    if (!m_artistFilter.isEmpty()) {
        m_artistFilteredMusicModel->addFilter(Nepomuk2::Vocabulary::NMM::performer(), Nepomuk2::Query::ResourceTerm(m_artistFilter));
    }
}

void MetadataMusicBackend::searchAlbum(const QString& album)
{
    m_albumsModel->setSearchTerm(album);
}

void MetadataMusicBackend::searchArtist(const QString& artist)
{
    m_artistsModel->setSearchTerm(artist);
}

void MetadataMusicBackend::searchMusic(const QString& music)
{
    m_musicModel->setSearchTerm(music);
}

void MetadataMusicBackend::searchArtistsMusic(const QString& music)
{
    m_artistFilteredMusicModel->setSearchTerm(music);
}

void MetadataMusicBackend::addAllSongsToPlaylist (QObject* playlistModel)
{
    m_shallAddMediaToPlaylist = true;
    m_playlistModel = qobject_cast<PlaylistModel*>(playlistModel);
    if (!m_playlistModel)
        return;
    for (int i=0; i<m_musicModel->rowCount(); ++i) {
        const QString url = m_musicModel->data(m_musicModel->index(i), MediaCenter::MediaUrlRole).toString();
        const QString name = m_musicModel->data(m_musicModel->index(i), Qt::DisplayRole).toString();
        if (!url.isEmpty() && !name.isEmpty()) {
            m_playlistModel->addToPlaylist(url);
        }
    }

    disconnect(m_musicModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(musicModelDataChanged(QModelIndex,QModelIndex)));
    connect(m_musicModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), SLOT(musicModelDataChanged(QModelIndex,QModelIndex)));
}

void MetadataMusicBackend::musicModelDataChanged ( const QModelIndex &startIndex, const QModelIndex &endIndex )
{
    if ( !(startIndex.isValid() && endIndex.isValid() && m_shallAddMediaToPlaylist ))
        return;
    for (int i=startIndex.row(); i<=endIndex.row(); ++i) {
        const QString url = m_musicModel->data(m_musicModel->index(i), MediaCenter::MediaUrlRole).toString();
        const QString name = m_musicModel->data(m_musicModel->index(i), Qt::DisplayRole).toString();
        if (!url.isEmpty() && !name.isEmpty()) {
            m_playlistModel->addToPlaylist(url);
        }
    }
}

void MetadataMusicBackend::musicModelReset()
{
    disconnect(m_musicModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(musicModelDataChanged(QModelIndex,QModelIndex)));
}

void MetadataMusicBackend::stopAddingSongsToPlaylist()
{
    m_shallAddMediaToPlaylist = false;
}

bool MetadataMusicBackend::expand(int row, QAbstractItemModel* model)
{
    if (!model) return false;

    const QString filter = model->data(model->index(row, 0), MediaCenter::ResourceIdRole).toString();

    if (model == m_albumsModel) {
        setAlbumFilter(filter);
        emit modelNeedsAttention(m_musicModel);
    } else if (model == m_artistsModel) {
        setArtistFilter(filter);
        emit modelNeedsAttention(m_artistFilteredMusicModel);
    }

    return true;
}

QVariantList MetadataMusicBackend::buttons()
{
    QVariantList buttonList;
    buttonList << s_showAllButton << s_playAllButton;
    return buttonList;
}

void MetadataMusicBackend::handleButtonClick(const QString& buttonName)
{
    if (buttonName == s_showAllButton) {
        m_albumFilter = m_artistFilter = "";
        updateModelAccordingToFilters();
    } else if (buttonName == s_playAllButton) {
        //FIXME: This is a horrible hack to get a ref to the playlist model
        PlaylistModel *model = qobject_cast<PlaylistModel*>(declarativeEngine()->rootContext()->contextProperty("playlistModel").value<QObject*>());
        if (model) {
            addAllSongsToPlaylist(model);
        } else {
            kWarning() << "Failed to get a reference to playlist model";
        }
    }
}

void MetadataMusicBackend::searchModel(const QString& searchTerm, QAbstractItemModel* model)
{
    if (model == m_musicModel) {
        searchMusic(searchTerm);
    } else if (model == m_artistFilteredMusicModel) {
        searchArtistsMusic(searchTerm);
    } else if (model == m_artistsModel) {
        searchArtist(searchTerm);
    } else if (model == m_albumsModel) {
        searchAlbum(searchTerm);
    }
}

#include "metadatamusicbackend.moc"
