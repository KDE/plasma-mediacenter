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

#include "mediacenter/abstractbrowsingbackend.h"

class FilterMediaModel;
class PmcMetadataModel;
class PmcModel;

class MetadataMusicBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
    Q_PROPERTY(QString artistFilter READ artistFilter NOTIFY artistFilterChanged)
    Q_PROPERTY(QString albumFilter READ albumFilter NOTIFY albumFilterChanged)

public:
    MetadataMusicBackend (QObject* parent, const QVariantList& args);
    virtual ~MetadataMusicBackend();

    QString artistFilter() const;
    QString albumFilter() const;

    void setArtistData(const QMap<int, QVariant>& data);
    void setAlbumData(const QMap<int, QVariant>& data);

    virtual bool expand(int row, QAbstractItemModel* model);

    virtual void handleButtonClick(const QString& buttonName);

    //virtual void searchModel(const QString& searchTerm, QAbstractItemModel* model);

    virtual QStringList allMedia();

public Q_SLOTS:
    bool back(QObject* model);

Q_SIGNALS:
    void artistFilterChanged();
    void albumFilterChanged();

protected:
    void updateModelAccordingToFilters();
    virtual bool initImpl();

private Q_SLOTS:
    void initializeModels();

private:
    PmcMetadataModel* m_artistsModel;
    PmcMetadataModel* m_albumsModel;
    PmcMetadataModel* m_musicModel;

    QString m_artistFilter;
    QString m_albumFilter;
    PmcModel* m_musicPmcModel;
    PmcModel* m_albumsPmcModel;
    PmcModel* m_artistsPmcModel;
    PmcModel* m_musicFilteredByAlbumPmcModel;
    PmcModel* m_musicFilteredByArtistPmcModel;

    FilterMediaModel* m_musicFilteredModel;
    FilterMediaModel* m_albumFilteredModel;
    FilterMediaModel* m_musicFilteredByAlbumModel;
    FilterMediaModel* m_artistFilteredModel;
    FilterMediaModel* m_musicFilteredByArtistModel;
};

#endif // METADATAMUSICBACKEND_H
