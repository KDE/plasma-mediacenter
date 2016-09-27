/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/

#include "filesystemsearchmediasource.h"

#include <singletonfactory.h>
#include <medialibrary.h>
#include <kmediacollection.h>

#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QMimeType>
#include <QMimeDatabase>

KMEDIACOLLECTION_EXPORT_DATASOURCE(FilesystemSearchMediaSource, "filesystemsearch.json")

FilesystemSearchMediaSource::FilesystemSearchMediaSource(QObject* parent, const QVariantList& args)
    : AbstractDataSource(parent, args),
      m_quit(false)
{
    m_allowedMimes << "audio" << "image" << "video";
}

FilesystemSearchMediaSource::~FilesystemSearchMediaSource()
{
    qDebug() << "Waiting for Filesystem Search to quit...";
    stop();
    wait();
}

void FilesystemSearchMediaSource::run()
{
    setPriority(IdlePriority);
    recursiveSearch(QDir::home());
    qDebug() << "Done scanning the Filesystem";
}

bool FilesystemSearchMediaSource::recursiveSearch(const QDir &dir)
{
    if (shouldQuit())   return false;

    Q_FOREACH(const QFileInfo &fileInfo, dir.entryInfoList(QDir::Files)) {
        if (shouldQuit())   return false;
        checkAndAddFile(fileInfo);
    }

    Q_FOREACH(const QFileInfo &dirInfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        if (shouldQuit())   return false;
        if (!recursiveSearch(QDir(dirInfo.absoluteFilePath()))) {
            break;
        }
    }

    return true;
}

bool FilesystemSearchMediaSource::shouldQuit() const
{
    QMutexLocker l(&m_quitMutex);
    return m_quit;
}

void FilesystemSearchMediaSource::stop()
{
    QMutexLocker l(&m_quitMutex);
    m_quit = true;
}

bool FilesystemSearchMediaSource::checkAndAddFile(const QFileInfo& fileInfo)
{
    QMimeDatabase db;
    const QString mimeType = db.mimeTypeForFile(fileInfo).name();
    const QString topLevelTypeName = mimeType.split('/').at(0);

    if (!topLevelTypeName.isEmpty() && m_allowedMimes.contains(topLevelTypeName)) {
        addFile(fileInfo, topLevelTypeName);
        return true;
    }

    return false;
}

void FilesystemSearchMediaSource::addFile(const QFileInfo& fileInfo,
                                          const QString &type)
{
    QHash<int, QVariant> values;

    values.insert(Qt::DisplayRole, fileInfo.fileName());
    values.insert(KMediaCollection::MediaUrlRole, QUrl::fromLocalFile(fileInfo.absoluteFilePath()).toString());
    values.insert(KMediaCollection::MediaTypeRole, type);

    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
}

#include "filesystemsearchmediasource.moc"
