/*
    Copyright (C) 2011  Shantanu Tushar shantanu@kde.org
    Copyright (C) 2012  Sinny Kumari ksinny@gmail.com

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* Part of this code was taken from project Taglib, originally licensed under
 * the BSD-2-clause license. You can find the original version at
 * https://github.com/taglib/taglib/tree/master/examples
*/

#include "localmusicmodel.h"

#include <KDE/KDirModel>
#include <mediacenter/mediacenter.h>

LocalMusicModel::LocalMusicModel (QObject* parent) : LocalFilesAbstractModel (parent, QString("audio/"))
{

}

QVariant LocalMusicModel::data(const QModelIndex& index, int role) const
{
    if (index.row() >= rowCount()) {
        return QVariant();
    }
    if (role == Qt::DecorationRole && !data(index, MediaCenter::IsExpandableRole).toBool()) {
        const QString url = data(index, MediaCenter::MediaUrlRole).toString();

        QImage image = imageForFile(QUrl(url).toLocalFile());
        m_wasImageFound.insert(index.row(), !image.isNull());
        if(image.isNull()) {
            return LocalFilesAbstractModel::data(index, role);
        } else {
            return image;
        }
    }
    if (role == MediaCenter::DecorationTypeRole && !data(index, MediaCenter::IsExpandableRole).toBool()) {
        return m_wasImageFound[index.row()] ? "qimage" : QVariant();
    }

    return LocalFilesAbstractModel::data(index, role);
}

QImage LocalMusicModel::imageForFile(const QString& file) const
{
    TagLib::MPEG::File f(file.toUtf8().constData());

    if(!f.isValid() || !f.ID3v2Tag())
        return QImage();

    return imageForTag(f.ID3v2Tag());
}

QImage LocalMusicModel::imageForTag(TagLib::ID3v2::Tag* tag) const
{
    TagLib::ID3v2::FrameList l = tag->frameList("APIC");
    QImage image;

    if(l.isEmpty()) {
        return image;
    }
    TagLib::ID3v2::AttachedPictureFrame *f = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(l.front());
    image.loadFromData((const uchar *) f->picture().data(), f->picture().size());
    return image;
}

#include "localmusicmodel.moc"
