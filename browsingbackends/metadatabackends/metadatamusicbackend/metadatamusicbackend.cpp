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
#include "metadatamusicsongsmodel.h"
#include "../metadatabackendcommonmodel.h"

#include "mediacenter/pmcmetadatamodel.h"
#include "mediacenter/playlistmodel.h"
#include "mediacenter/filtermediamodel.h"

#include <KDebug>

#include <QQmlEngine>
#include <QTimer>

MEDIACENTER_EXPORT_BROWSINGBACKEND2(MetadataMusicBackend, "metadatamusicbackend.json")

namespace {
    static const char *s_showAllButton = "Show All";
    static const char *s_playAllButton = "Play All";
}

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
    , m_artistsModel(0)
    , m_albumsModel(0)
    , m_musicModel(0)
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
    m_musicModel = new MetadataMusicSongsModel(this);
    m_artistsModel->setDefaultDecoration("user-identity");
    m_albumsModel->setDefaultDecoration("pmcnocover");
    connect(m_musicModel, SIGNAL(modelReset()), SLOT(musicModelReset()));

    m_musicFilteredModel = new FilterMediaModel(this);
    m_musicFilteredModel->setSourceModel(m_musicModel);
    m_musicModelMetadata = new ModelMetadata(m_musicFilteredModel, this);

    m_albumFilteredModel = new FilterMediaModel(this);
    m_albumFilteredModel->setSourceModel(m_albumsModel);
    m_albumsModelMetadata = new ModelMetadata(m_albumFilteredModel, this);

    m_artistFilteredModel = new FilterMediaModel(this);
    m_artistFilteredModel->setSourceModel(m_artistsModel);
    m_artistsModelMetadata = new ModelMetadata(m_artistFilteredModel, this);

    m_albumsModelMetadata->setName("Albums");
    m_artistsModelMetadata->setName("Artists");
    m_musicModelMetadata->setName("Songs#list");

    m_musicModelMetadata->setSupportsSearch(true);
    m_artistsModelMetadata->setSupportsSearch(true);
    m_albumsModelMetadata->setSupportsSearch(true);

    addModel(m_musicModelMetadata);
    addModel(m_albumsModelMetadata);
    addModel(m_artistsModelMetadata);

    QTimer::singleShot(1000, this, SLOT(initializeModels()));
    return true;
}

void MetadataMusicBackend::initializeModels()
{
    m_albumsModel->showAlbums();
    m_artistsModel->showArtist();
    m_musicModel->showMediaType(MediaCenter::Music);
    updateModelAccordingToFilters();
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
    FilterMediaModel *filterModel = static_cast<FilterMediaModel *> (m_musicFilteredModel);
    filterModel->clearFilters();

    if (!m_albumFilter.isEmpty()) {
        filterModel->setFilter(MediaCenter::AlbumRole, m_albumFilter);
    }
    if (!m_artistFilter.isEmpty()) {
        filterModel->setFilter(MediaCenter::ArtistRole, m_artistFilter);
    }
}

void MetadataMusicBackend::addAllSongsToPlaylist (QObject* playlistModelObject)
{
    PlaylistModel* playlistModel = static_cast<PlaylistModel *> (playlistModelObject);
    for (int i=0; i<m_musicFilteredModel->rowCount(); ++i) {
        const QString url = m_musicFilteredModel->data(m_musicFilteredModel->index(i, 0), MediaCenter::MediaUrlRole).toString();
        const QVariantList songAndItsInfo = m_musicFilteredModel->data(m_musicFilteredModel->index(i, 0), Qt::DisplayRole).toList();
        const QString name = songAndItsInfo.length() ? songAndItsInfo.at(0).toString() : QString();
        if (!url.isEmpty() && !name.isEmpty()) {
            playlistModel->addToPlaylist(url);
        }
    }
}

bool MetadataMusicBackend::expand(int row, QAbstractItemModel* model)
{
    if (!model) return false;

    const QString filter = model->data(model->index(row, 0), Qt::DisplayRole).toString();

    if (model == m_albumFilteredModel) {
        setAlbumFilter(filter);
        emit modelNeedsAttention(m_musicModelMetadata);
    } else if (model == m_artistFilteredModel) {
        setArtistFilter(filter);
        emit modelNeedsAttention(m_musicModelMetadata);
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
        /***
         * TODO Revisit this logic
        PlaylistModel *model = qobject_cast<PlaylistModel*>(declarativeEngine()->rootContext()->contextProperty("playlistModel").value<QObject*>());
        if (model) {
            addAllSongsToPlaylist(model);
            if (pmcRuntime()) {
                QObject *playlist = pmcRuntime()->property("playlist").value<QObject*>();
                QMetaObject::invokeMethod(playlist, "playNext");
            }
        } else {
            kWarning() << "Failed to get a reference to playlist model";
        }*/
    }
}

void MetadataMusicBackend::searchModel(const QString& searchTerm, QAbstractItemModel* model)
{
    static_cast<FilterMediaModel *> (model)->setFilter(Qt::DisplayRole, searchTerm);
}

#include "metadatamusicbackend.moc"
