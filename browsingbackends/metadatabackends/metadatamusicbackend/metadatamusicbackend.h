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

#include <libs/mediacenter/abstractbrowsingbackend.h>

#include "../abstractmetadatabackend.h"
#include "categoriesmodel.h"

#include <QStateMachine>

class CategoriesModel;
class NepomukMusicModel;
class MetadataMusicModel;

class MetadataMusicBackend : public AbstractMetadataBackend
{
    Q_OBJECT
    Q_PROPERTY(bool level1Visible READ level1Visible NOTIFY level1VisibleChanged)
    Q_PROPERTY(bool level2Visible READ level2Visible NOTIFY level2VisibleChanged)
    Q_PROPERTY(bool level3Visible READ level3Visible NOTIFY level3VisibleChanged)

public:
    MetadataMusicBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataMusicBackend();

    virtual void init();

    virtual QString mediaBrowserOverride() const;

    Q_INVOKABLE QObject *level1Model() const;
    Q_INVOKABLE QObject *level2Model() const;
    Q_INVOKABLE QObject *level3Model() const;

    Q_INVOKABLE void setCategory(int index);
    Q_INVOKABLE void setSubCategory(int index);

    bool level1Visible() const;
    bool level2Visible() const;
    bool level3Visible() const;

private Q_SLOTS:
    void showAllMusic();
    void showArtists();
    void showAlbums();
    void showMusicForArtist();
    void showMusicForAlbum();

Q_SIGNALS:
    void currentStateCategories();
    void currentStateAllMusic();
    void currentStateArtists();
    void currentStateAlbums();
    void currentStateMusicForArtist();
    void currentStateMusicForAlbum();
    void backRequested();

    void level1VisibleChanged();
    void level2VisibleChanged();
    void level3VisibleChanged();

private:
    QString m_artistName;
    QString m_albumName;

    CategoriesModel *m_categoriesModel;
    NepomukMusicModel *m_nepomukModel;
    MetadataMusicModel *m_metadataMusicModel;

    Category::CategoryType m_currentCategory;

    bool m_level1Visible;
    bool m_level2Visible;
    bool m_level3Visible;
};

#endif // METADATAMUSICBACKEND_H
