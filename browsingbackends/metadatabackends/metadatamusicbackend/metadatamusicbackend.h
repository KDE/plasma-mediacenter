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
    Q_PROPERTY(QString artistFilter READ artistFilter WRITE setArtistFilter NOTIFY artistFilterChanged)
    Q_PROPERTY(QString albumFilter READ albumFilter WRITE setAlbumFilter NOTIFY albumFilterChanged)
    Q_PROPERTY(QObject* musicModel READ musicModel NOTIFY musicModelChanged)

public:
    MetadataMusicBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataMusicBackend();

    virtual void init();
    virtual QString backendCategory() const;

    virtual QString mediaBrowserOverride() const;

    Q_INVOKABLE QObject *artistsModel() const;
    Q_INVOKABLE QObject *albumsModel() const;
    QObject *musicModel() const;

    bool supportsSearch() const;

    QString artistFilter() const;
    QString albumFilter() const;

    void setArtistFilter(const QString &filter);
    void setAlbumFilter(const QString &filter);

Q_SIGNALS:
    void artistFilterChanged();
    void albumFilterChanged();
    void musicModelChanged();

protected:
    void updateModelAccordingToFilters();

private:
    NepomukMusicModel *m_nepomukModel;
    NepomukMusicModel* m_artistsModel;
    NepomukMusicModel* m_albumsModel;
    NepomukMusicModel* m_musicModel;
    MetadataMusicModel *m_metadataMusicModel;

    QString m_artistFilter;
    QString m_albumFilter;
};

#endif // METADATAMUSICBACKEND_H
