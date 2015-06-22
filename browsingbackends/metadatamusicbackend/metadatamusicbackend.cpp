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
#include "mediacenter/pmcmodel.h"

#include <QDebug>
#include <klocalizedstring.h>

#include <QQmlEngine>
#include <QTimer>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend, "metadatamusicbackend.json")

namespace {
    static const QString s_showAllButton = i18nd("plasma-mediacenter", "Show All");
    static const QString s_playAllButton = i18nd("plasma-mediacenter", "Play All");
}

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractBrowsingBackend(parent, args)
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
    m_albumsModel = new PmcMetadataModel(this);
    m_artistsModel = new PmcMetadataModel(this);
    m_musicModel = new PmcMetadataModel(this);

    m_musicModel->setDefaultDecoration("image://icon/audio");
    m_artistsModel->setDefaultDecoration("image://icon/user-identity");
    m_albumsModel->setDefaultDecoration("image://icon/pmcnocover");

    m_musicFilteredModel = new FilterMediaModel(this);
    m_musicFilteredModel->setSourceModel(m_musicModel);
    m_musicPmcModel = new PmcModel(m_musicFilteredModel, this);
    m_musicPmcModel->setName(i18nd("plasma-mediacenter", "Songs"));
    m_musicPmcModel->setIsList(true);
    m_musicPmcModel->setViewType("listbrowser");

    m_albumFilteredModel = new FilterMediaModel(this);
    m_albumFilteredModel->setSourceModel(m_albumsModel);
    m_albumsPmcModel = new PmcModel(m_albumFilteredModel, this);
    m_albumsPmcModel->setName(i18nd("plasma-mediacenter", "Albums"));
    m_albumsPmcModel->setViewType("listbrowser");
    m_musicFilteredByAlbumModel = new FilterMediaModel(this);
    m_musicFilteredByAlbumModel->setSourceModel(m_musicModel);
    m_musicFilteredByAlbumPmcModel = new PmcModel(m_musicFilteredByAlbumModel);
    m_musicFilteredByAlbumPmcModel->setIsList(true);
    m_musicFilteredByAlbumPmcModel->setExpanded(true);
    m_musicFilteredByAlbumPmcModel->setViewType("listbrowser");

    m_artistFilteredModel = new FilterMediaModel(this);
    m_artistFilteredModel->setSourceModel(m_artistsModel);
    m_artistsPmcModel = new PmcModel(m_artistFilteredModel, this);
    m_artistsPmcModel->setName(i18nd("plasma-mediacenter", "Artists"));
    m_artistsPmcModel->setViewType("listbrowser");
    m_musicFilteredByArtistModel = new FilterMediaModel(this);
    m_musicFilteredByArtistModel->setSourceModel(m_musicModel);
    m_musicFilteredByArtistPmcModel = new PmcModel(m_musicFilteredByArtistModel);
    m_musicFilteredByArtistPmcModel->setIsList(true);
    m_musicFilteredByArtistPmcModel->setExpanded(true);
    m_musicFilteredByArtistPmcModel->setViewType("listbrowser");

    m_musicPmcModel->setSupportsSearch(true);
    m_artistsPmcModel->setSupportsSearch(true);
    m_albumsPmcModel->setSupportsSearch(true);

    addModel(m_musicPmcModel);
    addModel(m_albumsPmcModel);
    addModel(m_artistsPmcModel);

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

void MetadataMusicBackend::updateModelAccordingToFilters()
{
    if (!m_albumFilter.isEmpty()) {
        m_musicFilteredByAlbumModel->clearFilters();
        m_musicFilteredByAlbumModel->setFilter(MediaCenter::AlbumRole, m_albumFilter);
        m_musicFilteredByAlbumPmcModel->setName(m_albumFilter);
        replaceModel(m_albumsPmcModel, m_musicFilteredByAlbumPmcModel);
    }
    if (!m_artistFilter.isEmpty()) {
        m_musicFilteredByArtistModel->clearFilters();
        m_musicFilteredByArtistModel->setFilter(MediaCenter::ArtistRole, m_artistFilter);
        m_musicFilteredByArtistPmcModel->setName(m_artistFilter);
        replaceModel(m_artistsPmcModel, m_musicFilteredByArtistPmcModel);
    }
}

void MetadataMusicBackend::setArtistData(const QMap<int, QVariant>& data)
{
    m_musicFilteredByArtistPmcModel->setParentData(data);
}

void MetadataMusicBackend::setAlbumData(const QMap<int, QVariant>& data)
{
    m_musicFilteredByAlbumPmcModel->setParentData(data);
}

bool MetadataMusicBackend::expand(int row, QAbstractItemModel* model)
{
    if (!model) return false;

    const auto parentData = model->itemData(model->index(row, 0));

    const QString filter = model->data(model->index(row, 0), Qt::DisplayRole).toString();

    if (model == m_albumFilteredModel) {
        m_albumFilter = filter;
        setAlbumData(parentData);
        m_artistFilter.clear();
    } else if (model == m_artistFilteredModel) {
        m_artistFilter = filter;
        setArtistData(parentData);
        m_albumFilter.clear();
    }

    updateModelAccordingToFilters();

    emit artistFilterChanged();
    emit albumFilterChanged();

    return true;
}

bool MetadataMusicBackend::back(QObject* model)
{
    auto pmcModel = static_cast<PmcModel*>(model);
    if (!pmcModel) {
        return false;
    }
    if (pmcModel == m_musicFilteredByAlbumPmcModel) {
        return replaceModel(m_musicFilteredByAlbumPmcModel, m_albumsPmcModel);
    }
    if (pmcModel == m_musicFilteredByArtistPmcModel) {
        return replaceModel(m_musicFilteredByArtistPmcModel, m_artistsPmcModel);
    }
    return false;
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

//void MetadataMusicBackend::searchModel(const QString& searchTerm, QAbstractItemModel* model)
//{
//    static_cast<FilterMediaModel *> (model)->setFilter(Qt::DisplayRole, searchTerm);
//}

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
