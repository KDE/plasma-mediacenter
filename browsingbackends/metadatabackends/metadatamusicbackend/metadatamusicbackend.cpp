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

#include <libs/mediacenter/pmcmetadatamodel.h>

#include <nepomuk/nmm.h>
#include <nepomuk/nfo.h>
#include <nepomuk/andterm.h>
#include <nepomuk/comparisonterm.h>
#include <nepomuk/resourcetypeterm.h>
#include <nepomuk/resourceterm.h>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend)

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args)
{
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

void MetadataMusicBackend::initImpl()
{
    AbstractMetadataBackend::initImpl();
    m_albumsModel = new PmcMetadataModel(this);
    m_artistsModel = new PmcMetadataModel(this);
    m_musicModel = new PmcMetadataModel(this);
    m_albumsModel->showMediaForProperty(Nepomuk::Vocabulary::NMM::musicAlbum());
    m_artistsModel->showMediaForProperty(Nepomuk::Vocabulary::NMM::performer());
    m_musicModel->showMediaType(MediaCenter::Music);
    emit musicModelChanged();

    updateModelAccordingToFilters();
}

QString MetadataMusicBackend::mediaBrowserOverride() const
{
    return constructQmlSource("metadatamusiccomponents", "0.1", "MediaBrowser");
}

bool MetadataMusicBackend::supportsSearch() const
{
    return false;
}

QObject* MetadataMusicBackend::albumsModel() const
{
    return m_albumsModel;
}

QObject* MetadataMusicBackend::artistsModel() const
{
    return m_artistsModel;
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
    QList<Nepomuk::Query::Term> termsList;
    termsList.append(Nepomuk::Query::ResourceTypeTerm(Nepomuk::Vocabulary::NFO::Audio()));

    if (!m_albumFilter.isEmpty()) {
        m_musicModel->addFilter(Nepomuk::Vocabulary::NMM::musicAlbum(), Nepomuk::Query::ResourceTerm(m_albumFilter));
    }
    if (!m_artistFilter.isEmpty()) {
        m_musicModel->addFilter(Nepomuk::Vocabulary::NMM::performer(), Nepomuk::Query::ResourceTerm(m_artistFilter));
    }
}

QObject* MetadataMusicBackend::musicModel() const
{
    return m_musicModel;
}

QString MetadataMusicBackend::backendCategory() const
{
    return "audio";
}

#include "metadatamusicbackend.moc"
