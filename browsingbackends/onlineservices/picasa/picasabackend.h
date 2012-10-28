/***************************************************************************
 *   Copyright 2009 by Francesco Grieco <fgrieco@gmail.com>                *
 *   Copyright 2012 by Sinny Kumari <ksinny@gmail.com>                     *
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

#ifndef PICASABACKEND_H
#define PICASABACKEND_H

#include <libs/mediacenter/abstractbrowsingbackend.h>

/**
 * @class PicasaBackend
 * @brief A backend that manages queries to Picasa.
*/
class PicasaBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
public:
    PicasaBackend(QObject *parent, const QVariantList &args);
    virtual ~PicasaBackend();
    virtual QString backendCategory() const;
    virtual void search(const QString& searchTerm);
    virtual bool supportsSearch() const;
    virtual QString mediaBrowserSidePanel() const;
    Q_INVOKABLE void userInfo(const QString& username, const QString& password);
    bool expand (int row);

public Q_SLOTS:
    virtual void init();
    virtual bool goOneLevelUp();
};

#endif
