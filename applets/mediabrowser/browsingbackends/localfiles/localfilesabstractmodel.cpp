/*
    Copyright (C) 2011  Shantanu Tushar <jhahoneyk@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "localfilesabstractmodel.h"

#include <mediacenter/mediacenter.h>

#include <KDirModel>
#include <KDirLister>

class LocalFilesAbstractModel::Private
{
public:
    QStringList mimeTypes;
};

LocalFilesAbstractModel::LocalFilesAbstractModel (QObject* parent, const QString& acceptedMimetypes)
    : KDirModel (parent)
    , d(new Private())
{
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
    KMimeType::List mimeList = KMimeType::allMimeTypes();

    d->mimeTypes << "inode/directory";
    foreach (KMimeType::Ptr mime, mimeList) {
        if (mime->name().startsWith(acceptedMimetypes)) {
            d->mimeTypes << mime->name();
        }
    }

    if (dirLister()) {
        dirLister()->setMimeFilter(d->mimeTypes);
    }

    dirLister()->openUrl(KUrl::fromLocalFile(QDir::homePath()));
}

QVariant LocalFilesAbstractModel::data (const QModelIndex& index, int role) const
{
    switch (role) {
        case MediaCenter::IsExpandableRole:
            return data(index, KDirModel::FileItemRole).value<KFileItem>().isDir();
        case MediaCenter::MediaUrlRole:
            return data(index, KDirModel::FileItemRole).value<KFileItem>().url().prettyUrl();
        case MediaCenter::MediaTypeRole:
            KFileItem fileItem = data(index, KDirModel::FileItemRole).value<KFileItem>();
            const QString mime = fileItem.mimetype().split('/').at(0);

            if (mime == "audio" || mime == "image" || mime == "video")
                return mime;
    }
    return KDirModel::data (index, role);
}

bool LocalFilesAbstractModel::goOneLevelUp()
{
    KUrl url = dirLister()->url();

    if (QDir(url.toLocalFile()) == QDir(QDir::homePath())) {
        return false;
    }

    url.addPath("..");
    bool success = dirLister()->openUrl(url);

    return success;
}

bool LocalFilesAbstractModel::browseTo (int row)
{
    KUrl url = dirLister()->url();
    url.addPath(data(index(row, 0)).toString());
    bool success = dirLister()->openUrl(url);

    return success;
}

#include "localfilesabstractmodel.moc"
