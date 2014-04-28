/*
    Copyright (C) 2011  Shantanu Tushar <shantanu@kde.org>

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

#include "mediacenter/mediacenter.h"

#include <KDirModel>
#include <KDirLister>
#include <KMimeType>

#include <QStack>

class LocalFilesAbstractModel::Private
{
public:

    QStringList mimeTypes;
    KDirModel dirModel;
    QStack<QUrl> stack;

};

LocalFilesAbstractModel::LocalFilesAbstractModel (QObject* parent, const QString& acceptedMimetypes)
    : KDirSortFilterProxyModel (parent)
    , d(new Private())
{

    KMimeType::List mimeList = KMimeType::allMimeTypes();

    d->mimeTypes << "inode/directory";
    foreach (KMimeType::Ptr mime, mimeList) {
        if (mime->name().startsWith(acceptedMimetypes)) {
            d->mimeTypes << mime->name();
        }
    }

    if (d->dirModel.dirLister()) {
        d->dirModel.dirLister()->setMimeFilter(d->mimeTypes);
    }


    setSourceModel(&d->dirModel);
    setSortFoldersFirst(true);

    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));
}

QVariant LocalFilesAbstractModel::data (const QModelIndex& index, int role) const
{
    switch (role) {
        case MediaCenter::IsExpandableRole:
            return data(index, KDirModel::FileItemRole).value<KFileItem>().isDir();
        case MediaCenter::MediaUrlRole:
            return data(index, KDirModel::FileItemRole).value<KFileItem>().url().url();
        case MediaCenter::MediaTypeRole:
            KFileItem fileItem = data(index, KDirModel::FileItemRole).value<KFileItem>();
            const QString mime = fileItem.mimetype().split('/').at(0);

            if (mime == "audio" || mime == "image" || mime == "video")
                return mime;
    }
    return KDirSortFilterProxyModel::data (index, role);
}

bool LocalFilesAbstractModel::goOneLevelUp()
{
    d->stack.pop();
    if(d->stack.isEmpty()) {
        return false;
    }
    else {
    QUrl url = d->dirModel.dirLister()->url();
    url.setPath((d->stack.top()).toLocalFile());
    return d->dirModel.dirLister()->openUrl(url);
    }
}

bool LocalFilesAbstractModel::browseTo (int row)
{
    QUrl url = d->dirModel.dirLister()->url();
    url = url.adjusted(QUrl::StripTrailingSlash);
    url.setPath(url.path() + '/' + data(index(row, 0)).toString());
    return browseToUrl(url);
}

bool LocalFilesAbstractModel::browseToUrl(const QUrl& url)
{
    d->stack.push(url);
    return d->dirModel.dirLister()->openUrl(url);
}

#include "localfilesabstractmodel.moc"
