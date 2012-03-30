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

#include "metadatamusicmodel.h"
#include "nepomukmusicmodel.h"

#include "categoriesmodel.h"
#include "nepomukmusicmodel.h"
#include <nepomuk/nmm.h>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend)

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args),
    m_categoriesModel(new CategoriesModel(this)),
    m_nepomukModel(new NepomukMusicModel(this)),
    m_metadataMusicModel(0),
    m_currentCategory(Category::AllMusic),
    m_level1Visible(true),
    m_level2Visible(false),
    m_level3Visible(false)
{
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

void MetadataMusicBackend::setCategory (int index)
{
    m_currentCategory = m_categoriesModel->categoryTypeForIndex(index);
    switch (m_currentCategory) {
    case Category::AllMusic:
        showAllMusic();
        break;
    case Category::Artists:
        showArtists();
        break;
    case Category::Albums:
        showAlbums();
        break;
    }
}

void MetadataMusicBackend::setSubCategory (int index)
{
    QString currentItemLabel = m_nepomukModel->data(m_nepomukModel->index(index, 0), Qt::DisplayRole).toString();
    switch (m_currentCategory) {
    case Category::AllMusic:
        break;
    case Category::Artists:
        m_artistName = currentItemLabel;
        showMusicForArtist();
        break;
    case Category::Albums:
        m_albumName = currentItemLabel;
        showMusicForAlbum();
        break;
    }
}

void MetadataMusicBackend::init()
{
    if (metadataModel()) {
        m_metadataMusicModel = new MetadataMusicModel(this);
    }
    AbstractMetadataBackend::init();
}

void MetadataMusicBackend::showAllMusic()
{
    m_metadataMusicModel->setAlbumName("");
    m_metadataMusicModel->setArtistName("");
    m_metadataMusicModel->updateModel();

    m_level2Visible = false;
    emit level2VisibleChanged();
    m_level3Visible = true;
    emit level3VisibleChanged();
}

void MetadataMusicBackend::showAlbums()
{
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::musicAlbum(), "tools-media-optical-copy");

    m_level2Visible = true;
    emit level2VisibleChanged();
    m_level3Visible = false;
    emit level3VisibleChanged();
}

void MetadataMusicBackend::showArtists()
{
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::performer(), "user-identity");

    m_level2Visible = true;
    emit level2VisibleChanged();
    m_level3Visible = false;
    emit level3VisibleChanged();
}

void MetadataMusicBackend::showMusicForAlbum()
{
    m_metadataMusicModel->setArtistName("");
    m_metadataMusicModel->setAlbumName(m_albumName);
    m_metadataMusicModel->updateModel();

    m_level3Visible = true;
    emit level3VisibleChanged();
}

void MetadataMusicBackend::showMusicForArtist()
{
    m_metadataMusicModel->setArtistName(m_artistName);
    m_metadataMusicModel->setAlbumName("");
    m_metadataMusicModel->updateModel();

    m_level3Visible = true;
    emit level3VisibleChanged();
}

QString MetadataMusicBackend::mediaBrowserOverride() const
{
    return constructQmlSource("metadatamusiccomponents", "0.1", "MediaBrowser");
}

QObject* MetadataMusicBackend::level1Model() const
{
    return m_categoriesModel;
}

QObject* MetadataMusicBackend::level2Model() const
{
    return m_nepomukModel;
}

QObject* MetadataMusicBackend::level3Model() const
{
    return m_metadataMusicModel;
}

bool MetadataMusicBackend::level1Visible() const
{
    return m_level1Visible;
}

bool MetadataMusicBackend::level2Visible() const
{
    return m_level2Visible;
}

bool MetadataMusicBackend::level3Visible() const
{
    return m_level3Visible;
}

#include "metadatamusicbackend.moc"
