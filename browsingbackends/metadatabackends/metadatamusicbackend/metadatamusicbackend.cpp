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
    m_metadataMusicModel(0)
{
    setupStates();
}

void MetadataMusicBackend::setupStates()
{
    m_machine.addState(&m_categoriesState);
    m_machine.addState(&m_allMusicState);
    m_machine.addState(&m_albumsState);
    m_machine.addState(&m_artistsState);
    m_machine.addState(&m_musicForAlbumState);
    m_machine.addState(&m_musicForArtistState);

    connect(&m_musicForArtistState, SIGNAL(entered()), SLOT(showMusicForArtist()));
    connect(&m_musicForAlbumState, SIGNAL(entered()), SLOT(showMusicForAlbum()));
    connect(&m_categoriesState, SIGNAL(entered()), SLOT(showCategories()));
    connect(&m_allMusicState, SIGNAL(entered()), SLOT(showAllMusic()));
    connect(&m_artistsState, SIGNAL(entered()), SLOT(showArtists()));
    connect(&m_albumsState, SIGNAL(entered()), SLOT(showAlbums()));

    m_categoriesState.addTransition(this, SIGNAL(currentStateAllMusic()), &m_allMusicState);
    m_categoriesState.addTransition(this, SIGNAL(currentStateArtists()), &m_artistsState);
    m_categoriesState.addTransition(this, SIGNAL(currentStateAlbums()), &m_albumsState);

    m_allMusicState.addTransition(this, SIGNAL(backRequested()), &m_categoriesState);
    m_artistsState.addTransition(this, SIGNAL(backRequested()), &m_categoriesState);
    m_albumsState.addTransition(this, SIGNAL(backRequested()), &m_categoriesState);

    m_machine.setInitialState(&m_categoriesState);
}

MetadataMusicBackend::~MetadataMusicBackend()
{
}

void MetadataMusicBackend::init()
{
    if (metadataModel()) {
        m_metadataMusicModel = new MetadataMusicModel(this);
    }
    AbstractMetadataBackend::init();
    m_machine.start();
}

QState* MetadataMusicBackend::currentState() const
{
    return qobject_cast<QState*>(m_machine.configuration().values().at(0));
}

bool MetadataMusicBackend::expand(int row)
{
    QState *curState = currentState();

    if (curState == &m_categoriesState) {
        switch (m_categoriesModel->categoryTypeForIndex(row)) {
        case Category::AllMusic:
            emit currentStateAllMusic();
            return true;
        case Category::Artists:
            emit currentStateArtists();
            return true;
        case Category::Albums:
            emit currentStateAlbums();
            return true;
        }
    }

    return false;
}

bool MetadataMusicBackend::goOneLevelUp()
{
    if (currentState() == &m_categoriesState) {
        return false;
    }

    emit backRequested();
    return true;
}

void MetadataMusicBackend::showCategories()
{
    setModel(m_categoriesModel);
}

void MetadataMusicBackend::showAllMusic()
{
    setModel(m_metadataMusicModel);
}

void MetadataMusicBackend::showAlbums()
{
    setModel(m_nepomukModel);
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::musicAlbum());
}

void MetadataMusicBackend::showArtists()
{
    setModel(m_nepomukModel);
    m_nepomukModel->setTerm(Nepomuk::Vocabulary::NMM::performer());
}

void MetadataMusicBackend::showMusicForAlbum()
{

}

void MetadataMusicBackend::showMusicForArtist()
{

}

#include "metadatamusicbackend.moc"
