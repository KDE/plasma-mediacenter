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

#include <mediacenter/singletonfactory.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <KMimeType>

#include <QDir>
#include <QDebug>

MEDIACENTER_EXPORT_MEDIASOURCE(FilesystemSearchMediaSource)

FilesystemSearchMediaSource::FilesystemSearchMediaSource(QObject* parent, const QVariantList&)
    : m_quit(false)
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
    const QString mimeType = KMimeType::findByUrl(
        KUrl::fromLocalFile(fileInfo.absoluteFilePath()))->name();

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
    qDebug() << "Adding " << type << fileInfo.fileName();
    QHash<int, QVariant> values;

    values.insert(Qt::DisplayRole, fileInfo.fileName());
    values.insert(MediaCenter::MediaUrlRole, QUrl::fromLocalFile(fileInfo.absoluteFilePath()).toString());
    values.insert(MediaCenter::MediaTypeRole, type);

    SingletonFactory::instanceFor<MediaLibrary>()->updateMedia(values);
}
