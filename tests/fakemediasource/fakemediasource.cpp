/*
 *   Copyright 2015 Ashish Bansal <bansal.ashish096@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "fakemediasource.h"

#include <mediacenter/singletonfactory.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <QFile>
#include <QTimer>
#include <QByteArray>
#include <QJsonArray>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QDebug>
#include <QStringList>

MEDIACENTER_EXPORT_MEDIASOURCE(FakeMediaSource, "fakemediasource.json")

FakeMediaSource::FakeMediaSource(QObject* parent, const QVariantList& args)
: AbstractMediaSource(parent, args)
{

}

void FakeMediaSource::run()
{
    QTimer::singleShot(0, this, SLOT(parseJsonFile()));
    exec();
}

void FakeMediaSource::parseJsonFile()
{
    QString medialistFilePath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, getMediaFileName());
    if (medialistFilePath.isEmpty()) {
        return;
    }

    QFile jsonFile(medialistFilePath);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open json medialist file.");
        return;
    }

    QByteArray rawJsonData = jsonFile.readAll();
    QJsonArray mediaList = QJsonDocument::fromJson(rawJsonData).array();
    for(QJsonArray::Iterator media = mediaList.begin(); media != mediaList.end(); media++) {
        QJsonObject mediaprop = (*media).toObject();
        QHash<int, QString> properties;
        properties.insert(Qt::DisplayRole, mediaprop.value("title").toString());
        properties.insert(MediaCenter::AlbumRole, mediaprop.value("album").toString());
        properties.insert(MediaCenter::MediaUrlRole, mediaprop.value("url").toString());
        properties.insert(MediaCenter::ArtistRole, mediaprop.value("artist").toString());
        properties.insert(4444, mediaprop.value("mimeType").toString());
        properties.insert(MediaCenter::DurationRole, mediaprop.value("duration").toString());
        properties.insert(MediaCenter::AlbumArtistRole, mediaprop.value("albumArtist").toString());
        addMedia(properties);
    }
}

inline QString FakeMediaSource::getMediaFileName()
{
    return "plasma/mediacenter/mediasources/medialist.json";
}

void FakeMediaSource::addMedia(QHash< int, QString > properties)
{
    QString rawUrl = properties.value(MediaCenter::MediaUrlRole);
    QString mediaType = properties.value(4444);
    QHash <int, QVariant> values;
    values.insert(Qt::DisplayRole, QVariant(properties.value(Qt::DisplayRole)));
    values.insert(MediaCenter::MediaUrlRole, QVariant(rawUrl));
    values.insert(MediaCenter::MediaTypeRole, QVariant(mediaType));

    if (mediaType == "audio") {
        values.insert(MediaCenter::DurationRole, properties.value(MediaCenter::DurationRole));
        values.insert(MediaCenter::ArtistRole, properties.value(MediaCenter::ArtistRole));
        values.insert(MediaCenter::AlbumRole, properties.value(MediaCenter::AlbumRole));
        values.insert(MediaCenter::AlbumArtistRole, properties.value(MediaCenter::AlbumArtistRole));
    } else if (mediaType == "video") {
        values.insert(MediaCenter::DurationRole, properties.value(MediaCenter::DurationRole));
    }
    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(rawUrl, values);
}

#include "fakemediasource.moc"
