/* Copyright (C) 2004 Scott Wheeler <wheeler@kde.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
    Copyright (C) 2011  Shantanu Tushar shaan7in@gmail.com
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


#include "localmusicmodel.h"

#include <KDE/KDirModel>
#include <libs/mediacenter/mediacenter.h>

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
