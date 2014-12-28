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

#include "mediacenter/pmcmetadatamodel.h"
#include "mediacenter/playlistmodel.h"
#include "mediacenter/filtermediamodel.h"
#include "mediacenter/modelmetadata.h"

#include <KDebug>
#include <klocalizedstring.h>

#include <QQmlEngine>
#include <QTimer>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend, "metadatamusicbackend.json")

namespace {
    static const QString s_showAllButton = i18n("Show All");
    static const QString s_playAllButton = i18n("Play All");
}

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
    , m_artistsModel(0)
    , m_albumsModel(0)
    , m_musicModel(0)
{
    setButtons(QStringList() << s_showAllButton << s_playAllButton);
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

bool MetadataMusicBackend::initImpl()
{
    AbstractMetadataBackend::initImpl();
    m_albumsModel = new PmcMetadataModel(this);
    m_artistsModel = new PmcMetadataModel(this);
    m_musicModel = new PmcMetadataModel(this);
    m_musicModel->setDefaultDecoration("image://icon/audio");
    m_artistsModel->setDefaultDecoration("image://icon/user-identity");
    m_albumsModel->setDefaultDecoration("image://icon/pmcnocover");

    m_musicFilteredModel = new FilterMediaModel(this);
    m_musicFilteredModel->setSourceModel(m_musicModel);
    m_musicModelMetadata = new ModelMetadata(m_musicFilteredModel, this);

    m_albumFilteredModel = new FilterMediaModel(this);
    m_albumFilteredModel->setSourceModel(m_albumsModel);
    m_albumsModelMetadata = new ModelMetadata(m_albumFilteredModel, this);

    m_artistFilteredModel = new FilterMediaModel(this);
    m_artistFilteredModel->setSourceModel(m_artistsModel);
    m_artistsModelMetadata = new ModelMetadata(m_artistFilteredModel, this);

    m_albumsModelMetadata->setName(i18n("Albums"));
    m_artistsModelMetadata->setName(i18n("Artists"));
    m_musicModelMetadata->setName(i18n("Songs"));

    m_musicModelMetadata->setSupportsSearch(true);
    m_musicModelMetadata->setIsList(true);
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

void MetadataMusicBackend::handleButtonClick(const QString& buttonName)
{
    if (buttonName == s_showAllButton) {
        m_albumFilter = m_artistFilter = "";
        updateModelAccordingToFilters();
    } else if (buttonName == s_playAllButton) {
        //FIXME: Logic for playing all media
    }
}

void MetadataMusicBackend::searchModel(const QString& searchTerm, QAbstractItemModel* model)
{
    static_cast<FilterMediaModel *> (model)->setFilter(Qt::DisplayRole, searchTerm);
}

QStringList MetadataMusicBackend::allMedia()
{
    QStringList mediaList;
    for (int i=0; i<m_musicFilteredModel->rowCount(); ++i) {
        const auto index = m_musicFilteredModel->index(i, 0);
        const auto url = m_musicFilteredModel->data(index, MediaCenter::MediaUrlRole).toString();
        const auto songAndItsInfo = m_musicFilteredModel->data(index, Qt::DisplayRole).toList();
        const auto name = songAndItsInfo.length() ? songAndItsInfo.at(0).toString() : QString();
        if (!url.isEmpty() && !name.isEmpty()) {
            mediaList.append(url);
        }
    }
    return mediaList;
}

#include "metadatamusicbackend.moc"
