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

#include "picasabackend.h"
#include "picasamodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(PicasaBackend)

PicasaBackend::PicasaBackend(QObject* parent, const QVariantList& args):
                             MediaCenter::AbstractBrowsingBackend(parent, args)
{

}

QString PicasaBackend::backendCategory() const
{
    return "image";
}

bool PicasaBackend::initImpl()
{
    return true;
}

bool PicasaBackend::goOneLevelUp()
{
    PicasaModel *picasaModel = qobject_cast<PicasaModel*>(model());
    return picasaModel->goBack();
}

QString PicasaBackend::mediaBrowserSidePanel() const
{
    return constructQmlSource("picasacomponents", "0.1", "PicasaSidePanel");
}

void PicasaBackend::login(const QString& username, const QString& password)
{
    setModel(new PicasaModel(this, username, password));
}

bool PicasaBackend::expand(int row)
{
    PicasaModel *picasaModel = qobject_cast<PicasaModel*>(model());
    return picasaModel->browseToAlbum(row);
}
