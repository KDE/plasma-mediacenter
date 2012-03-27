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

#include <QStateMachine>
#include <QDebug>

MEDIACENTER_EXPORT_BROWSINGBACKEND(MetadataMusicBackend)

MetadataMusicBackend::MetadataMusicBackend(QObject* parent, const QVariantList& args)
    : AbstractMetadataBackend(parent, args),
    m_categoriesModel(new CategoriesModel(this)),
    m_nepomukModel(new NepomukMusicModel(this)),
    m_metadataMusicModel(0),
    m_currentCategory(Category::AllMusic)
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
qDebug() << currentItemLabel;
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
    m_machine.start();
}

void MetadataMusicBackend::showAllMusic()
{
    m_metadataMusicModel->setAlbumName("");
    m_metadataMusicModel->setArtistName("");
    m_metadataMusicModel->updateModel();
}

void MetadataMusicBackend::showAlbums()
{
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::musicAlbum(), "tools-media-optical-copy");
}

void MetadataMusicBackend::showArtists()
{
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::performer(), "user-identity");
}

void MetadataMusicBackend::showMusicForAlbum()
{
    m_metadataMusicModel->setArtistName("");
    m_metadataMusicModel->setAlbumName(m_albumName);
    m_metadataMusicModel->updateModel();
}

void MetadataMusicBackend::showMusicForArtist()
{
    m_metadataMusicModel->setArtistName(m_artistName);
    m_metadataMusicModel->setAlbumName("");
    m_metadataMusicModel->updateModel();
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

#include "metadatamusicbackend.moc"
