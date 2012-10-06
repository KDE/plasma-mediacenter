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



PicasaBackend::~PicasaBackend()
{

}

QString PicasaBackend::backendCategory() const
{
    return "image";
}

void PicasaBackend::init()
{
//     setModel(new PicasaModel(this));
}

bool PicasaBackend::goOneLevelUp()
{
    return MediaCenter::AbstractBrowsingBackend::goOneLevelUp();
}

void PicasaBackend::search(const QString& searchTerm)
{
    MediaCenter::AbstractBrowsingBackend::search(searchTerm);
}

bool PicasaBackend::supportsSearch() const
{
    return true;
}

QString PicasaBackend::mediaBrowserSidePanel() const
{
    return constructQmlSource("picasacomponents", "0.1", "PicasaSidePanel");
}

void PicasaBackend::userInfo(const QString& username, const QString& password)
{
    setModel(new PicasaModel(this, username, password));
}


// bool PicasaEngine::sourceRequestEvent(const QString &name)
// {
//     QString queryString = name;
//     QString request;
//
//     // the syntax to require the list of albums is
//     // album/user@gmail.com:password
//     // ex: album/user@gmail.com:password
//     if (name.startsWith("album/")) {
//         queryString.remove("album/");
//         request = "album";
//     // the syntax to require the photos of an album is
//     // photo/albumID/user@gmail.com:password
//     // ex: photo/32323232432/user@gmail.com:password
//     } else if (name.startsWith("photo/")) {
//         queryString.remove("photo/");
//         QStringList list = queryString.split("/");
//         queryString.remove(list.first() + "/");
//         request = "photo/" + list.first();
//     } else {
//         return false;
//     }
//
//     QString password;
//     if (queryString.contains(":")) {
//         QStringList list = queryString.split(":");
//         // if we require photos from an album, the albumid is contained
//         // in request
//         queryString = list.first();
//         password = list.last();
//         m_interface->getTokenAndQuery(queryString, password, request);
//     } else {
//         m_interface->query(queryString, request);
//     }
//
//     return true;
// }
//
// void PicasaEngine::slotSourceFromResult(const QString &searchTerm, const QString &id, const Plasma::DataEngine::Data &album)
// {
//     setData(searchTerm, id, album);
// }
//
// K_EXPORT_PLASMA_DATAENGINE(picasa, PicasaEngine)
